/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/25 16:09:08 by jasper        ########   odam.nl         */
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

#include <time.h>	// no-norm

#define KEY_ESC 65307
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_Q 113
#define KEY_E 101

#define KEY_R 114
#define KEY_F 102

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

int	hook_key_down(int key,void *p)
{
	t_mlx_data* data = p;
	printf("Key down: %i\n", key);

	if (key == KEY_ESC)
		mlx_loop_end(data->mlx);
	else if (key == KEY_W)
		data->input.forward = true;
	else if (key == KEY_A)
		data->input.left = true;
	else if (key == KEY_S)
		data->input.backward = true;
	else if (key == KEY_D)
		data->input.right = true;
	else if (key == KEY_Q)
		data->input.up = true;
	else if (key == KEY_E)
		data->input.down = true;
	else if (key == KEY_R)
		data->input.white_up = true;
	else if (key == KEY_F)
		data->input.white_down = true;
	return 0;
}

int	hook_key_up(int key,void *p)
{
	t_mlx_data* data = p;
	printf("Key up: %i\n", key);

	if (key == KEY_W)
		data->input.forward = false;
	else if (key == KEY_A)
		data->input.left = false;
	else if (key == KEY_S)
		data->input.backward = false;
	else if (key == KEY_D)
		data->input.right = false;
	else if (key == KEY_Q)
		data->input.up = false;
	else if (key == KEY_E)
		data->input.down = false;
	else if (key == KEY_R)
		data->input.white_up = false;
	else if (key == KEY_F)
		data->input.white_down = false;
	return 0;
}

/*
**	1 (left click) = look at point
**	2 (middle click) = gather debug data from pixel
**	3 (right click) = get HDR color
*/

int hook_mouse(int button, int x, int y, void* p)
{
	t_mlx_data* data = p;

	//printf("mouse hooked! %i at: (%i, %i) %p\n",button, x, y, p);
	if (button == 1)
	{
		t_ray ray;
		pix_to_ray(data, x, y, &ray);

		t_quaternion new_rot = quaternion_from_forward_up(ray.direction, vec3_new(0,1,0));
		t_camera* cam = darray_index(&data->scene->cameras, data->scene->current_camera_index);
		cam->transform.rotation = new_rot;
	}
	if (button == 2)
	{
		t_ray ray;
		t_ray_hit hit;

		pix_to_ray(data, x, y, &ray);
		if (trace_ray(data, &ray, &hit))
		{
			printf("Hit!\n");
			ft_printf("Location: %v!\n", hit.location);
			ft_printf("Normal: %v!\n", hit.normal);
			ft_printf("Color: %v!\n", *(t_vec3*)&hit.color);

			float sqrmag = vec3_magnitude_sqr(hit.normal);
			if (sqrmag > 1.01 || sqrmag < 0.99)
				printf("Normal magnitude != 1, actual: %.2f\n", sqrtf(sqrmag));
		}
		else
			printf("Miss!\n");
	}
	else if (button == 3)
	{
		t_color_hdr hdr = data->pixels[x + y * data->scene->resolution.width].color;
		printf("Color: %.2f %.2f %.2f, mag: %.2f\n", hdr.r, hdr.g, hdr.b,
			sqrtf(hdr.r * hdr.r + hdr.g * hdr.g + hdr.b * hdr.b)
		);
	}
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

void trace_pixel(t_mlx_data* data, int x, int y)
{
	t_color_hdr* hdr = &data->pixels[x + y * data->scene->resolution.width].color;
	t_ray ray;
	pix_to_ray(data, x, y, &ray);
	trace_color(data, &ray, hdr);
	update_pix(data, x, y);
}

void trace_next_pixel(t_mlx_data* data)
{
	int pix = data->current_pixel;
	data->current_pixel++;
	if (data->current_pixel >= data->scene->resolution.width * data->scene->resolution.height)
	{
		printf("Completed frame!\n");
		data->current_pixel = 0;
	}
	trace_pixel(data, pix % data->scene->resolution.width, pix / data->scene->resolution.width);
}

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

	// Input
	t_vec3 move_dir = vec3_new(0,0,0);
	move_dir.z = (data->input.forward ? -1 : 0) + (data->input.backward ? 1 : 0);
	move_dir.x = (data->input.right ? -1 : 0) + (data->input.left ? 1 : 0);
	move_dir.y = (data->input.down ? -1 : 0) + (data->input.up ? 1 : 0);

	t_camera* cam = darray_index(&data->scene->cameras, data->scene->current_camera_index);
	move_dir = quaternion_mult_vec3(cam->transform.rotation, vec3_scale(move_dir, diff / (float)CLOCKS_PER_SEC * 2.0));
	cam->transform.position = vec3_add(cam->transform.position, move_dir);

	// Render
	clock_t stop = clock() + CLOCKS_PER_SEC / 60;
	while (clock() < stop)
	{
		for (int i = 0; i < 1000; i++)
			trace_next_pixel(data);
	}

	if (data->input.white_up != data->input.white_down)
	{
		data->white += ((data->input.white_down ? -1 : 0) + (data->input.white_up ? 1 : 0))
					* diff / (float)CLOCKS_PER_SEC * 2.0;
		if (data->white < 0)
			data->white = 0;
		update_image(data);
	}

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
	init_ft_printf();

	t_args* arg_data = parse_args(argc, argv);
	if (!arg_data)
	{
		ft_printf("Error\n%s\n",get_last_error());
		return 1;
	}

	ft_printf("File: %s\n", arg_data->map_file);
	ft_printf("Save: %i\n", arg_data->save);

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
		return 1;
	}

	for (size_t i = 0; i < scene->objects.count; i++)
	{
		t_object* obj = darray_index(&scene->objects, i);
		t_vec3 x = quaternion_mult_vec3(obj->transform.rotation, vec3_new(1,0,0));
		t_vec3 y = quaternion_mult_vec3(obj->transform.rotation, vec3_new(0,1,0));
		t_vec3 z = quaternion_mult_vec3(obj->transform.rotation, vec3_new(0,0,1));
		ft_printf("Found object with transform %t with x (%v) y: (%v) z: (%v) \n",
			obj->transform,
			x, y, z
		);
	}

	void* mlx = mlx_init();
	if (!mlx)
	{
		free_scene(scene);
		free(arg_data);
		write(STDOUT_FILENO, "Error\nCould not init mlx!\n", 26);
		return 1;
	}

	int rx, ry;
	mlx_get_screen_size(mlx, &rx, &ry);
	if (scene->resolution.width > rx)
		scene->resolution.width = rx;
	if (scene->resolution.height > ry)
		scene->resolution.height = ry;

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
	ft_bzero(&mlx_data.input, sizeof(t_input));
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

	mlx_hook(window, KeyPress, KeyPressMask, &hook_key_down, &mlx_data);
	mlx_hook(window, KeyRelease, KeyReleaseMask, &hook_key_up, &mlx_data);
	mlx_loop_hook(mlx, hook_loop, &mlx_data);
	mlx_mouse_hook(window, hook_mouse, &mlx_data);

	mlx_hook(window, ClientMessage, NoEventMask, hook_client_message, &mlx_data);

	//mlx_do_key_autorepeatoff(mlx);	// TODO: Add to hook mouse enter, or maybe not
	//mlx_do_key_autorepeaton(mlx);	// TODO: Add to hook mouse leave, or maybe not

	mlx_loop(mlx);
	//trace_ray(&mlx_data, 0, 0);

	// TODO: if save arg specified, save image here

	mlx_destroy_window(mlx, window);
	mlx_destroy_image(mlx, mlx_data.img.image);
	free(arg_data);
	close(fd);
	write(STDOUT_FILENO, "Completed!\n", 11);
	return 0;
}