/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 20:45:03 by jasper        ########   odam.nl         */
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
		mlx_destroy_window(data->mlx, data->window);
		mlx_loop_end(data->mlx);
	}
	return 0;
}

int	hook_loop(void *p)
{
//	printf("Looping!\n");
	(void)p;
	return 0;
}

int hook_destroy_notify(void* p)
{
	t_mlx_data* data = p;
	mlx_destroy_window(data->mlx, data->window);
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

	t_mlx_data mlx_data;
	mlx_data.mlx = mlx;
	mlx_data.window = window;
	mlx_key_hook(window, hook_key, &mlx_data);
	mlx_loop_hook(mlx, hook_loop, &mlx_data);
	mlx_hook(window, ClientMessage, NoEventMask, hook_destroy_notify, &mlx_data);

	mlx_loop(mlx);

	free(data);
	close(fd);
	write(STDOUT_FILENO, "Completed!\n", 11);
	return 0;
}