/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/16 14:30:03 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mlx.h"
#include "mlx_int.h"
#include <math.h>
#include "ft_printf.h"
#include "ft_error.h"
#include <pthread.h>	// used for threading (bonus)
#include <time.h>	// used for movement (bonus)

#define NUM_THREADS 5

/*
**	TODO: Sync rendering
*/

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

void update_pix(t_mlx_data* data, int x, int y)
{
	t_color_hdr hdr = data->pixels[x + y * data->scene->resolution.width].color;
	t_color_rgb rgb = color_hdr_to_rgb_reindard_white(hdr, data->white);
	unsigned int col = (rgb.b | rgb.g << 8) | rgb.r << 16;

	size_t offset = x * (data->img.bits_per_pixel / 8) + y * data->img.line_length;
	unsigned int* addr = (unsigned int*)(data->img.addr + offset);
	*addr = col;
}

void update_image(t_mlx_data* data)
{
	// TODO: I should only update the pixels that have changed, this already takes 100 ms
	for (int x = 0; x < data->img.width; x++)
	{
		for (int y = 0; y < data->img.height; y++)
		{
			update_pix(data, x, y);
		}
	}
}

void trace_pixel(t_mlx_data* data, int x, int y)
{
	// I want this pixel to be divided into anti_aliasing^2 equally sized squares
	// every ray gets 1/anti_aliasing of width
	// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space) gets a range of 0 to 1
	// so, subtract 0.5 from that to get a range from -0.5 to 0.5
	t_color_hdr* hdr = &data->pixels[x + y * data->scene->resolution.width].color;
	t_ray ray;
	t_color_hdr current;
	hdr->r = 0;
	hdr->g = 0;
	hdr->b = 0;

	int aa = data->scene->anti_aliasing;
	float step = 1.0/aa;
	float offset = step / 2 - 0.5;
	for (int ox = 0; ox < aa; ox++)
	{
		for (int oy = 0; oy < aa; oy++)
		{
			pix_to_ray(data,
				x + step * ox + offset,
				y + step * ox + offset,
				&ray
			);
			trace_color(data, &ray, &current);
			hdr->r += current.r;
			hdr->g += current.g;
			hdr->b += current.b;
		}
	}

	float total_samples = aa * aa;
	hdr->r /= total_samples;
	hdr->g /= total_samples;
	hdr->b /= total_samples;
	update_pix(data, x, y);
}

void trace_next_pixels(t_mlx_data* data, int desired)
{
	pthread_mutex_lock(&data->lock);
	int pix = data->current_pixel;
	data->current_pixel+=desired;
	if (data->current_pixel >= data->scene->resolution.width * data->scene->resolution.height)
	{
		//printf("Completed frame!\n");
		data->current_pixel = 0;
	}
	pthread_mutex_unlock(&data->lock);
	int stop = pix+desired;
	if (stop > data->scene->resolution.width * data->scene->resolution.height)
		stop = data->scene->resolution.width * data->scene->resolution.height;
	for (int i = pix; i < stop; i++)
	{
		trace_pixel(data, i % data->scene->resolution.width, i / data->scene->resolution.width);
	}
}

void* new_thread(void* p)
{
	t_mlx_data* data = p;

	while (data->active)
	{
		trace_next_pixels(data, 1000);
	}
	return NULL;
}

/*
**	Beware! if you get more than 400 fps it gets REAL LAGGY...
**	What?
*/

int	hook_loop(void *p)
{
	t_mlx_data* data = p;

	// FPS printing
	static clock_t last_tick = 0;
	static clock_t time = 0;
	static int frames = 0;
	clock_t current = clock();
	clock_t diff = current - last_tick;
	last_tick = current;
	time += diff;
	frames++;
	if (time > CLOCKS_PER_SEC * NUM_THREADS)
	{
		printf("%i fps (%.4fs/f)\n", frames, (float)(time / frames) / (CLOCKS_PER_SEC * NUM_THREADS));
		time = 0;
		frames = 0;
	}

	// Input
	t_vec3 move_dir = (t_vec3) { 0, 0, 0 };
	move_dir.z = (data->input.forward ? -1 : 0) + (data->input.backward ? 1 : 0);
	move_dir.x = (data->input.right ? -1 : 0) + (data->input.left ? 1 : 0);
	move_dir.y = (data->input.down ? -1 : 0) + (data->input.up ? 1 : 0);

	t_camera* cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	quaternion_mult_vec3(&move_dir, &cam->transform.rotation, &move_dir);
	vec3_scale(&move_dir, &move_dir, diff / (float)CLOCKS_PER_SEC * 2.0);
	vec3_add(&cam->transform.position, &cam->transform.position, &move_dir);

	// Render
	trace_next_pixels(data, 1000 / (data->scene->anti_aliasing * data->scene->anti_aliasing));

	if (data->input.white_up != data->input.white_down)
	{
		float change = powf(1.1,
			((data->input.white_down ? -1 : 0) + (data->input.white_up ? 1 : 0))
			* (diff / (float)CLOCKS_PER_SEC * 10)
		);
		data->white *= change;
		if (data->white < 0.001)
			data->white = 0.001;
		update_image(data);
	}

	mlx_put_image_to_window(data->mlx, data->window, data->img.image, 0, 0);

	return 0;
}

bool save_image(t_mlx_image* img, char* path)
{
	unsigned char* pixels = malloc(img->width * img->height * 3);
	if (pixels == NULL)
	{
		set_error("Error\nCould not malloc pixels to create screenshot!\n", false);
		return false;
	}

	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
	if (fd == -1)
	{
		set_error(ft_strjoin("Could not open/create file: ", path), true);
		return false;
	}

	for (int i = 0; i < img->width * img->height; i++)
	{
		int x = i % img->width;
		int y = i / img->width;

		size_t offset = x * (img->bits_per_pixel / 8) + y * img->line_length;
		unsigned int col = *(unsigned int*)(img->addr + offset);
		// (rgb.b | rgb.g << 8) | rgb.r << 16;
		unsigned char r = (col >> 16) & 0xff;
		unsigned char g = (col >> 8 ) & 0xff;
		unsigned char b = (col >> 0 ) & 0xff;

		pixels[i * 3] = r;
		pixels[i * 3 + 1] = g;
		pixels[i * 3 + 2] = b;
	}

	if (!write_bmp(fd, pixels, img->width, img->height))
	{
		set_error(ft_strjoin("Could not write into file: ", path), true);
		return false;
	}
	free(pixels);
	close(fd);

	return true;
}

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
		pthread_create(&thread_ids[i], NULL, new_thread, &mlx_data);

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