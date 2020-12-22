/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 19:52:07 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

t_args* parse_args(int argc, char **argv)
{
	t_args* data = malloc(sizeof(t_args));
	if (data == NULL)
		return NULL;

	if (argc <= 1)
	{
		data->error_msg = ft_strdup("Usage: miniRT [file] {args}");
		if (data->error_msg == NULL)
		{
			free(data);
			return (NULL);
		}
		return data;
	}

	data->map_file = argv[1];
	if (ft_strlen(data->map_file) < 3 || ft_strncmp(data->map_file + ft_strlen(data->map_file) - 3,".rt", 3) != 0)
	{
		data->error_msg = ft_strjoin("File does not end with \".rt\", got: ", data->map_file);
		if (data->error_msg == NULL)
		{
			free(data);
			return (NULL);
		}
		return data;
	}


	int i = 2;
	while (i < argc)
	{
		char* arg = argv[i];
		if (ft_strncmp(arg, "--save", 7) == 0 && data->save == false)
			data->save = true;
		else
		{
			data->error_msg = ft_strjoin("Argument not recognized: ", arg);
			if (data->error_msg == NULL)
			{
				free(data);
				return (NULL);
			}
			return data;
		}
		i++;
	}

	data->error_msg = NULL;
	return data;
}

int main(int argc, char **argv)
{
	t_args* data = parse_args(argc, argv);
	if (!data)
	{
		write(STDOUT_FILENO, "Error\nMalloc Failed!\n", 21);
		return 1;
	}
	if (data->error_msg != NULL)
	{
		write(STDOUT_FILENO, "Error\n", 6);
		write(STDOUT_FILENO, data->error_msg, ft_strlen(data->error_msg));
		write(STDOUT_FILENO, "\n", 1);
		free(data);
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
		write(STDOUT_FILENO, "Error\nMalloc Failed!\n", 21);
		free(data);
		return 1;
	}

	free(data);
	close(fd);
	return 0;
}