/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/20 15:24:55 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "libft.h"
#include "mlx.h"
#include "mlx_int.h"
#include <math.h>
#include "ft_printf.h"
#include "ft_error.h"
#include <pthread.h>	// used for threading (bonus)
#include "ft_time.h"	// Used for movement (bonus)

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
		// TODO: This is not %100 thread safe
		//	Notify all render threads to pause
		//	Then wait untill all render threads have paused
		//	Then clear the pixels
		//	Then notify all render threads to un-pause
		//pthread_mutex_lock(&data->lock);
		data->should_clear = false;
		// If we have moved, we need to clear the pixels
		for (int i = 0; i < data->img.width * data->img.height; i++)
		{
			data->pixels[i].color = (t_color_hdr) { 0,0,0 };
			data->pixels[i].num_samples = 0;
		}
		//pthread_mutex_unlock(&data->lock);
	}
	// Render
	trace_next_pixels(data, 15000);

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

	int fd = open(arg_data->map_file, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error\nCould not open file \"%s\"!\n", arg_data->map_file);
		free(arg_data);
		return 1;
	}

	t_scene* scene = parse_scene_file(fd);
	if (!scene)
	{
		ft_printf("Error\nAn error occured while parsing the file: \"%s\"!\n", get_last_error());
		free(arg_data);
		close(fd);
		return 1;
	}

	/*
	for (size_t i = 0; i < scene->objects.count; i++)
	{
		t_object* obj = list_index(&scene->objects, i);
		t_vec3 x;
		t_vec3 y;
		t_vec3 z;

		quaternion_mult_vec3(&x, &obj->transform.rotation, vec3_right());
		quaternion_mult_vec3(&y, &obj->transform.rotation, vec3_up());
		quaternion_mult_vec3(&z, &obj->transform.rotation, vec3_back());
		ft_printf("Found object with transform %t with x (%v) y: (%v) z: (%v) \n",
			&obj->transform,
			&x, &y, &z
		);
	}
	*/

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
	mlx_data.pixels = malloc(sizeof(*mlx_data.pixels) * scene->resolution.width * scene->resolution.height);
	mlx_data.current_pixel = 0;
	mlx_data.white = 1;
	mlx_data.active = true;
	mlx_data.debug_trace_aabb = false;
	ft_bzero(&mlx_data.input, sizeof(t_input));
	if (mlx_data.pixels == NULL)
	{
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		ft_printf("Error\nCould not create pixel array!\n");
		return 1;
	}
	if (pthread_mutex_init(&mlx_data.lock, NULL) != 0)
	{
		free(mlx_data.pixels);
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
		free(mlx_data.pixels);
		free(arg_data);
		ft_printf("Error\nCould not create mlx image!\n");
		return 1;
	}
	for (int i = 0; i < scene->resolution.width * scene->resolution.height; i++)
	{
		t_color_hdr* col = &mlx_data.pixels[i].color;
		col->r = 0;
		col->g = 0;
		col->b = 0;
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
	pthread_mutex_destroy(&mlx_data.lock);

	if (arg_data->save_on_exit)
		if (!save_image(&mlx_data.img, "screenshot.bmp"))
			ft_printf("Error\nAn error occured while saving the screenshot: \"%s\"!\n", get_last_error());

	mlx_destroy_window(mlx, window);
	mlx_destroy_image(mlx, mlx_data.img.image);
	free(arg_data);
	close(fd);
	ft_printf("Completed!\n");
	return 0;
}