/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/21 14:22:58 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "libft.h"
#include "mlx.h"
#include "mlx_int.h"
#include <math.h>
#include "ft_printf.h"
#include "ft_error.h"
#include "ft_time.h"	// Used for movement (bonus)
#include "ft_manual_reset_event.h"

#define NUM_THREADS 5

bool init_image(void* mlx, t_mlx_image* img, int width, int height)
{
	void* image = mlx_new_image(mlx, width, height);
	if (!image)
		return false;
	img->image = image;
	img->width = width;
	img->height = height;
	img->addr = mlx_get_data_addr(image, &img->bits_per_pixel, &img->line_length, (int*)&img->big_endian);
	return true;
}

/*
**	returns time since last frame
*/

static float hook_frame()
{
	// 	Get time since last frame
	static t_time last = { 0, 0 };
	t_time current = time_now();
	float diff = time_difference(&current, &last);
	if (last.seconds == 0)
		diff = 0;
	last = current;

	//	Accumulate values
	static float time = 0;
	static int frames = 0;
	time += diff;
	frames++;
	//	Print out
	if (time > 1)
	{
		printf("%i fps (%.4fs/f)\n", frames, time / frames);
		time = 0;
		frames = 0;
	}
	return (diff);
}

/*
**	Beware! if you get more than 400 fps it gets REAL LAGGY...
**	What?
*/

int	hook_loop(void *p)
{
	t_mlx_data* data = p;

	float diff = hook_frame();

	// Input
	t_vec3 move_dir = (t_vec3) { 0, 0, 0 };
	data->should_clear = data->should_clear || (data->input.forward != data->input.backward)
					|| (data->input.right != data->input.left)
					|| (data->input.down != data->input.up);
	move_dir.z = (data->input.forward ? -1 : 0) + (data->input.backward ? 1 : 0);
	move_dir.x = (data->input.right ? -1 : 0) + (data->input.left ? 1 : 0);
	move_dir.y = (data->input.down ? -1 : 0) + (data->input.up ? 1 : 0);

	t_camera* cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	quaternion_mult_vec3(&move_dir, &cam->transform.rotation, &move_dir);
	vec3_scale(&move_dir, &move_dir, diff * 5);
	vec3_add(&cam->transform.position, &cam->transform.position, &move_dir);

	if (data->should_clear)
	{
		data->should_clear = false;

		// If we have moved, we need to clear the pixels
		pthread_mutex_lock(&data->renderer.start_thread_lock);	// Prevent new threads from starting
		manual_reset_event_wait(&data->renderer.no_render_threads_mre);	// Wait untill all render threads have stopped
		//data->renderer.current_pixel = 0;	// restart progress of frame, so we dont have stale pixels
		data->renderer.dirty_frame = true;	// Instead of resetting the progress of the frame, only getting updated pixels at the top, i mark this frame dirty: aka: next frame will also be a first_frame

		data->renderer.first_frame = true;	// Tell it to render at 1 spp, no sobel
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
	}
	// Render
	trace_next_pixels(data, 2500);

	if (data->input.white_up != data->input.white_down)
	{
		float change = powf(1.1,
			((data->input.white_down ? -1 : 0) + (data->input.white_up ? 1 : 0))
			* (diff * 2)
		);
		data->white *= change;
		if (data->white < 0.001)
			data->white = 0.001;
		update_image(data);
	}

	mlx_put_image_to_window(data->mlx, data->window, data->img.image, 0, 0);

	return 0;
}

static t_scene* parse_scene(char* path)
{
	t_scene* scene;

	int fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		set_error(ft_strjoin("Could not open file ", path), true);
		return (NULL);
	}

	scene = parse_scene_file(fd);
	if (!scene)
	{
		set_error(ft_strjoin("An error occured while parsing the file: ", get_last_error()), true);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (scene);
}

#include "libft.h"
#include <math.h>
int main(int argc, char **argv)
{
	init_ft_printf();

	t_args* arg_data = parse_args(argc, argv);
	if (!arg_data)
	{
		ft_printf("Error\n%s\n",get_last_error());
		return 1;
	}

	t_scene* scene = parse_scene(arg_data->map_file);
	if (!scene)
	{
		free(arg_data);
		ft_printf("Error\n%s\n",get_last_error());
		return 1;
	}

	void* mlx = mlx_init();
	if (!mlx)
	{
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not init mlx!\n");
		return 1;
	}

	if (!arg_data->no_res_cap)
	{
		int rx, ry;
		mlx_get_screen_size(mlx, &rx, &ry);
		if (scene->resolution.width > rx)
			scene->resolution.width = rx;
		if (scene->resolution.height > ry)
			scene->resolution.height = ry;
	}

	void* window = mlx_new_window(mlx, scene->resolution.width, scene->resolution.height, "miniRT");
	if (!window)
	{
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not create mlx window!\n");
		return 1;
	}

	t_mlx_data mlx_data;
	mlx_data.mlx = mlx;
	mlx_data.window = window;
	mlx_data.scene = scene;
	mlx_data.renderer.pixels = malloc(sizeof(*mlx_data.renderer.pixels) * scene->resolution.width * scene->resolution.height);
	mlx_data.renderer.temp_pixels = malloc(sizeof(*mlx_data.renderer.pixels) * scene->resolution.width * scene->resolution.height);
	mlx_data.renderer.current_pixel = 0;
	mlx_data.renderer.first_frame = true;
	mlx_data.renderer.dirty_frame = false;
	mlx_data.white = 1;
	mlx_data.active = true;
	mlx_data.debug_trace_aabb = false;
	ft_bzero(&mlx_data.input, sizeof(t_input));
	if (mlx_data.renderer.pixels == NULL || mlx_data.renderer.temp_pixels == NULL)
	{
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		free(mlx_data.renderer.pixels);
		free(mlx_data.renderer.temp_pixels);
		ft_printf("Error\nCould not create pixel array!\n");
		return 1;
	}
	// TODO: Destroy mutexes if failed
	if (pthread_mutex_init(&mlx_data.renderer.start_thread_lock, NULL) != 0)
	{
		free(mlx_data.renderer.pixels);
		free(mlx_data.renderer.temp_pixels);
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not init pthread mutex!\n");
		return 1;
	}
	if (pthread_mutex_init(&mlx_data.renderer.hook_thread_lock, NULL) != 0)
	{
		free(mlx_data.renderer.pixels);
		free(mlx_data.renderer.temp_pixels);
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not init pthread mutex!\n");
		return 1;
	}
	if (pthread_mutex_init(&mlx_data.renderer.active_render_threads_lock, NULL) != 0)
	{
		free(mlx_data.renderer.pixels);
		free(mlx_data.renderer.temp_pixels);
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not init pthread mutex!\n");
		return 1;
	}
	if (manual_reset_event_init(&mlx_data.renderer.no_render_threads_mre) != 0)
	{
		free(mlx_data.renderer.pixels);
		free(mlx_data.renderer.temp_pixels);
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not init pthread mutex!\n");
		return 1;
	}
	init_image(mlx, &mlx_data.img, scene->resolution.width, scene->resolution.height);
	if (!mlx_data.img.image)
	{
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(mlx_data.renderer.pixels);
		free(mlx_data.renderer.temp_pixels);
		free(arg_data);
		ft_printf("Error\nCould not create mlx image!\n");
		return 1;
	}
	for (int i = 0; i < scene->resolution.width * scene->resolution.height; i++)
	{
		t_pixel_data* pix = &mlx_data.renderer.pixels[i];
		pix->color = (t_color_hdr) { 0,0,0 };
		pix->num_samples = 0;
	}

	mlx_hook(window, KeyPress, KeyPressMask, &hook_key_down, &mlx_data);
	mlx_hook(window, KeyRelease, KeyReleaseMask, &hook_key_up, &mlx_data);
	mlx_loop_hook(mlx, hook_loop, &mlx_data);
	mlx_mouse_hook(window, hook_mouse, &mlx_data);

	mlx_hook(window, ClientMessage, StructureNotifyMask, hook_client_message, &mlx_data);

	pthread_t thread_ids[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&thread_ids[i], NULL, new_pixel_render_thread, &mlx_data);

	mlx_loop(mlx);
	mlx_data.active = false;

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(thread_ids[i], NULL);
	pthread_mutex_destroy(&mlx_data.renderer.start_thread_lock);
	pthread_mutex_destroy(&mlx_data.renderer.hook_thread_lock);
	pthread_mutex_destroy(&mlx_data.renderer.active_render_threads_lock);
	manual_reset_event_destroy(&mlx_data.renderer.no_render_threads_mre);

	if (arg_data->save_on_exit)
		if (!save_image(&mlx_data.img, "screenshot.bmp"))
			ft_printf("Error\nAn error occured while saving the screenshot: \"%s\"!\n", get_last_error());

	mlx_destroy_window(mlx, window);
	mlx_destroy_image(mlx, mlx_data.img.image);
	free_scene(scene);
	free(mlx_data.renderer.pixels);
	free(mlx_data.renderer.temp_pixels);
	free(arg_data);
	ft_printf("Completed!\n");
	return 0;
}