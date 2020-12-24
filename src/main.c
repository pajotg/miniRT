/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/24 14:12:01 by jasper        ########   odam.nl         */
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
	if (key == 65307)
	{
		t_mlx_data* data = p;
		mlx_loop_end(data->mlx);
	}
	return 0;
}

/*
**	TODO: Sync rendering
*/

/*
int	hook_expose(void *p)
{
	t_mlx_data* data = p;

	printf("Expose!\n");
	mlx_put_image_to_window(data->mlx, data->window, data->render_image.image, 0, 0);
	return 0;
}
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

/* Super simple render image function
static int x = 0;
static int red = 128;
for (int i = 0; i < 100; i++)
{
	x++;
	if (x == img.width)
	{
		x = 0;
		red += 64;
	}
	for (int y = 0; y < img.height; y++)
	{
		unsigned int col = (x | y << 8) | red << 16;
		size_t offset = x * (img.bits_per_pixel / 8) + y * img.line_length;
		unsigned int* addr = (unsigned int*)(img.addr + offset);
		*addr = col;
	}
}
*/

/*
** Why do i have to destroy and re-create the image for it to work!?
*/

int	hook_loop(void *p)
{
	t_mlx_data* data = p;

	t_mlx_image img;
	init_image(data->mlx, &img, data->scene->resolution.width, data->scene->resolution.height);

	// Render image here

	mlx_put_image_to_window(data->mlx, data->window, img.image, 0, 0);
	mlx_destroy_image(data->mlx, img.image);
	return 0;
}

int hook_destroy_notify(void* p)
{
	t_mlx_data* data = p;
	mlx_loop_end(data->mlx);
	return 0;
}

/*
** What event is bound to this button? lets print it out!
**for (int event = 0; event < 37; event++)
**{
**	for (int mask = 0; mask <= 24; mask++)
**	{
**		mlx_hook(window, event, (1L << mask), hook_test, (void*)(size_t)(event << 8 | mask));
**	}
**}
**int hook_test(void* p)
**{
**	printf("Got pointer: %p aka mask: %i and event: %i\n",p, (char)(size_t)p, (char)((size_t)p >> 8));
**	return 0;
**}
*/

int main(int argc, char **argv)
{
	t_args* data = parse_args(argc, argv);
	if (!data)
	{
		write(STDOUT_FILENO, "Error\n", 6);
		write(STDOUT_FILENO, get_last_error(), ft_strlen(get_last_error()));
		write(STDOUT_FILENO, "\n", 1);
		return 1;
	}

	printf("File: %s\n", data->map_file);
	printf("Save: %i\n", data->save);

	int fd = open(data->map_file, O_RDONLY);
	if (fd == -1)
	{
		write(STDOUT_FILENO, "Error\nCould not open file \"", 27);
		write(STDOUT_FILENO, data->map_file, ft_strlen(data->map_file));
		write(STDOUT_FILENO, "\"!\n", 3);
		free(data);
		return 1;
	}

	t_scene* scene = parse_scene_file(fd);
	if (!scene)
	{
		write(STDOUT_FILENO, "Error\nAn error occured while parsing the file: \"", 48);
		write(STDOUT_FILENO, get_last_error(), ft_strlen(get_last_error()));
		write(STDOUT_FILENO, "\"!\n", 3);
		free(data);
		return 1;
	}

	void* mlx = mlx_init();
	if (!mlx)
	{
		free_scene(scene);
		free(data);
		write(STDOUT_FILENO, "Error\nCould not init mlx!\n", 26);
		return 1;
	}

	void* window = mlx_new_window(mlx, scene->resolution.width, scene->resolution.height, "miniRT");
	if (!window)
	{
		free_scene(scene);
		free(data);
		write(STDOUT_FILENO, "Error\nCould not create mlx window!\n", 35);
		return 1;
	}

	void* image = mlx_new_image(mlx, scene->resolution.width, scene->resolution.height);
	if (!image)
	{
		mlx_destroy_window(mlx, window);
		free_scene(scene);
		free(data);
		write(STDOUT_FILENO, "Error\nCould not create mlx image!\n", 35);
		return 1;
	}

	t_mlx_data mlx_data;
	mlx_data.mlx = mlx;
	mlx_data.window = window;
	mlx_data.scene = scene;

	//mlx_expose_hook(window,hook_expose,&mlx_data);
	mlx_key_hook(window, hook_key, &mlx_data);
	mlx_loop_hook(mlx, hook_loop, &mlx_data);
	mlx_hook(window, ClientMessage, NoEventMask, hook_destroy_notify, &mlx_data);

	mlx_loop(mlx);

	// TODO: if save arg specified, save image here

	mlx_destroy_window(mlx, window);
	mlx_destroy_image(mlx, image);
	free(data);
	close(fd);
	write(STDOUT_FILENO, "Completed!\n", 11);
	return 0;
}