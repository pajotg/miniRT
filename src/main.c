/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/24 19:36:09 by jasper        ########   odam.nl         */
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


#include <time.h>	// no-norm

t_args* parse_args(int argc, char **argv)
{
	t_args* data = malloc(sizeof(t_args));
	if (data == NULL)
	{
		set_error("Malloc failed for parse args!", false);
		return NULL;
	}

	if (argc <= 1)
	{
		set_error("Usage: miniRT [file] {args}", false);
		free(data);
		return (NULL);
	}

	data->map_file = argv[1];
	if (ft_strlen(data->map_file) < 4 || ft_strncmp(data->map_file + ft_strlen(data->map_file) - 3,".rt", 3) != 0 || (data->map_file[ft_strlen(data->map_file)-4] == '/'))	// Checks for: strlen >= 4, ends with .rt, and character before . != /
	{
		set_error(ft_strjoin("File does not end with \".rt\", got: ", data->map_file), true);
		free(data);
		return NULL;
	}


	int i = 2;
	while (i < argc)
	{
		char* arg = argv[i];
		if (ft_strncmp(arg, "--save", 7) == 0 && data->save == false)
			data->save = true;
		else
		{
			set_error(ft_strjoin("Argument not recognized: ", arg), true);
			free(data);
			return NULL;
		}
		i++;
	}

	return data;
}

int	hook_key(int key,void *p)
{
	if (key == 65307)	//esc
	{
		t_mlx_data* data = p;
		mlx_loop_end(data->mlx);
	}
	return 0;
}

int hook_configure_notify(void* p)
{
	printf("Yeet %p\n", p);
	return 0;
}

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

/*
** acos(1 / dist) = cam.fov
** 1 / dist = cos(cam.fov)
** 1 = cos(cam.fov) * dist
** 1 / cos(cam.fov) = dist
*/

void trace_ray(t_mlx_data* data, int x, int y)
{
	t_color_hdr* hdr = &data->pixels[x + y * data->scene->resolution.width].color;
	float fov_axis = data->scene->resolution.width;
	float ox = (x - data->scene->resolution.width / 2.0) / fov_axis;
	float oy = (y - data->scene->resolution.height / 2.0) / fov_axis;

	// ox and oy are in the range -0.5 to 0.5

	t_camera* cam = darray_index(&data->scene->cameras, data->scene->current_camera_index);
	float dist = 1 / cos(cam->fov);

	t_vec3 dir = vec3_normalize(vec3_add(
		vec3_scale(quaternion_mult_vec3(cam->transform.rotation, vec3_new(1,0,0)), ox),
		vec3_scale(quaternion_mult_vec3(cam->transform.rotation, vec3_new(0,1,0)), oy)
	));

	t_ray ray;
	ray.origin = cam->transform.position;
	ray.direction = dir;

	t_ray_hit hit;
	hit.distance = INFINITY;
	bool has_hit = false;
	for (int i = 0; i < data->scene->objects.count; i++)
	{
		t_object* obj = darray_index(&data->scene->objects, i);
		if (obj->IntersectFunc(obj, &ray, &hit))
			has_hit = true;
	}
	if (has_hit)
		*hdr = hit.color;
	else
	{
		hdr->r = 0;
		hdr->g = 0;
		hdr->b = 0;
	}
	update_pix(data, x, y);
}

void trace_next_ray(t_mlx_data* data)
{
	int pix = data->current_pixel;
	data->current_pixel++;
	if (data->current_pixel >= data->scene->resolution.width * data->scene->resolution.height)
	{
		printf("Completed frame!\n");
		data->current_pixel = 0;
	}
	trace_ray(data, pix % data->scene->resolution.width, pix / data->scene->resolution.width);
}

/*
void update_image_lines(t_mlx_data* data, t_mlx_image* img)
{
	static int red = 128;
	for (int x = 0; x < img->width; x++)
	{
		for (int y = 0; y < img->height; y++)
		{
			unsigned int col = (x | y << 8) | red << 16;
			size_t offset = x * (img->bits_per_pixel / 8) + y * img->line_length;
			unsigned int* addr = (unsigned int*)(img->addr + offset);
			*addr = col;
		}
	}
	red += 64;
}
*/

/*
**	Beware! if you get more than 400 fps it gets REAL LAGGY...
**	What?
*/

int	hook_loop(void *p)
{
	// FPS printing
	static clock_t last_tick = 0;
	static clock_t time = 0;
	static int frames = 0;
	clock_t current = clock();
	clock_t diff = current - last_tick;
	last_tick = current;
	time += diff;
	frames++;
	if (time > CLOCKS_PER_SEC)
	{
		printf("%i fps (%.4fs/f)\n", frames, (float)(time / frames) / CLOCKS_PER_SEC);
		time = 0;
		frames = 0;
	}

	t_mlx_data* data = p;

	// Render
	clock_t stop = clock() + CLOCKS_PER_SEC / 60;
	while (clock() < stop)
	{
		for (int i = 0; i < 1000; i++)
			trace_next_ray(data);
	}
	/*
	data->white += (data->white) / 100 + 0.01;
	if (data->white > 5)
		data->white = 0;
	update_image(data);
	*/

	mlx_put_image_to_window(data->mlx, data->window, data->img.image, 0, 0);

	return 0;
}

int hook_client_message(void* p)
{
	t_mlx_data* data = p;
	mlx_loop_end(data->mlx);
	return 0;
}

/*
**	After reading the code, it seems like mlx_hook only uses the last mask specified for an event, previous values are overridden
**	I can actually change the source code from mlx to print out the events, so thats a much better solution
**	After seeing someone else mention the command: "xev"... why did i do that? THERE WAS A COMMAND
*/

int main(int argc, char **argv)
{
	t_args* arg_data = parse_args(argc, argv);
	if (!arg_data)
	{
		write(STDOUT_FILENO, "Error\n", 6);
		write(STDOUT_FILENO, get_last_error(), ft_strlen(get_last_error()));
		write(STDOUT_FILENO, "\n", 1);
		return 1;
	}

	printf("File: %s\n", arg_data->map_file);
	printf("Save: %i\n", arg_data->save);

	int fd = open(arg_data->map_file, O_RDONLY);
	if (fd == -1)
	{
		write(STDOUT_FILENO, "Error\nCould not open file \"", 27);
		write(STDOUT_FILENO, arg_data->map_file, ft_strlen(arg_data->map_file));
		write(STDOUT_FILENO, "\"!\n", 3);
		free(arg_data);
		return 1;
	}

	t_scene* scene = parse_scene_file(fd);
	if (!scene)
	{
		write(STDOUT_FILENO, "Error\nAn error occured while parsing the file: \"", 48);
		write(STDOUT_FILENO, get_last_error(), ft_strlen(get_last_error()));
		write(STDOUT_FILENO, "\"!\n", 3);
		free(arg_data);
		return 1;
	}

	for (int i = 0; i < scene->objects.count; i++)
	{
		t_object* obj = darray_index(&scene->objects, i);
		t_vec3 x = quaternion_mult_vec3(obj->transform.rotation, vec3_new(1,0,0));
		t_vec3 y = quaternion_mult_vec3(obj->transform.rotation, vec3_new(0,1,0));
		t_vec3 z = quaternion_mult_vec3(obj->transform.rotation, vec3_new(0,0,1));
		printf("Found object at: (%.2f %.2f %.2f) with x (%.2f %.2f %.2f) y: (%.2f %.2f %.2f) z: (%.2f %.2f %.2f) \n",
			obj->transform.position.x,obj->transform.position.y,obj->transform.position.z,
			x.x, x.y, x.z,
			y.x, y.y, y.z,
			z.x, z.y, z.z
		);
		printf("	Rot: (%.2f %.2f %.2f %.2f)\n", obj->transform.rotation.r, obj->transform.rotation.i, obj->transform.rotation.j, obj->transform.rotation.k);
	}

	void* mlx = mlx_init();
	if (!mlx)
	{
		free_scene(scene);
		free(arg_data);
		write(STDOUT_FILENO, "Error\nCould not init mlx!\n", 26);
		return 1;
	}

	void* window = mlx_new_window(mlx, scene->resolution.width, scene->resolution.height, "miniRT");
	if (!window)
	{
		free_scene(scene);
		free(arg_data);
		write(STDOUT_FILENO, "Error\nCould not create mlx window!\n", 35);
		return 1;
	}

	t_mlx_data mlx_data;
	mlx_data.mlx = mlx;
	mlx_data.window = window;
	mlx_data.scene = scene;
	mlx_data.pixels = malloc(sizeof(*mlx_data.pixels) * scene->resolution.width * scene->resolution.height);
	mlx_data.current_pixel = 0;
	mlx_data.white = 1;
	if (mlx_data.pixels == NULL)
	{
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(arg_data);
		write(STDOUT_FILENO, "Error\nCould not create pixel array!\n", 35);
		return 1;
	}
	init_image(mlx, &mlx_data.img, scene->resolution.width, scene->resolution.height);
	if (!mlx_data.img.image)
	{
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(mlx_data.pixels);
		free(arg_data);
		write(STDOUT_FILENO, "Error\nCould not create mlx image!\n", 35);
		return 1;
	}
	for (int i = 0; i < scene->resolution.width * scene->resolution.height; i++)
	{
		t_color_hdr* col = &mlx_data.pixels[i].color;
		col->r = 0;
		col->g = 0;
		col->b = 0;
	}

	for (int x = 0; x < scene->resolution.width; x++)
	{
		for (int y = 0; y < scene->resolution.height; y++)
		{
			t_color_hdr* hdr = &mlx_data.pixels[x + y * scene->resolution.width].color;
			hdr->r = x / 100.0;
			hdr->g = y / 100.0;
			hdr->b = (x+y) / 1000.0;
		}
	}

	mlx_key_hook(window, hook_key, &mlx_data);
	mlx_loop_hook(mlx, hook_loop, &mlx_data);
	mlx_hook(window, ClientMessage, NoEventMask, hook_client_message, &mlx_data);

	mlx_loop(mlx);

	// TODO: if save arg specified, save image here

	mlx_destroy_window(mlx, window);
	mlx_destroy_image(mlx, mlx_data.img.image);
	free(arg_data);
	close(fd);
	write(STDOUT_FILENO, "Completed!\n", 11);
	return 0;
}