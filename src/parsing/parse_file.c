/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_file.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 19:27:40 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 13:58:24 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_utils.h"
#include <stddef.h>
#include "mini_rt_utils.h"
#include "get_next_line.h"
#include <string.h>
#include <errno.h>
#include "libft.h"

#include <stdio.h>

bool parse_line(t_scene_parse_data* parse_data, t_scene* scene, char* line)
{
	int curr = 1;
	if (line[0] == 'R')
	{
		if (parse_data->has_resolution)
		{
			set_error(ft_strjoin("Duplicate resolution!: ", line), true);
			return false;
		}
		parse_data->has_resolution = true;
		skip_whitespace(line, &curr);
		scene->resolution.width = read_int(line, &curr);
		skip_whitespace(line, &curr);
		scene->resolution.height = read_int(line, &curr);
	}
	else if (line[0] == 'A')
	{
		if (parse_data->has_ambiant)
		{
			set_error(ft_strjoin("Duplicate ambiant!: ", line), true);
			return false;
		}
		parse_data->has_ambiant = true;
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, &scene->ambiant))
		{
			set_error(ft_strjoin("Ambiant incorrectly formatted!: ", line), true);
			return false;
		}
	}
	else if (line[0] == 'c')
	{
		t_camera camera;
		skip_whitespace(line, &curr);
		if (!read_transform(line, &curr, &camera.transform))
		{
			set_error(ft_strjoin("Camera transform incorrectly formatted!: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		int start = curr;
		camera.fov = read_float(line, &curr);
		if (curr == start)
		{
			set_error(ft_strjoin("Camera FOV incorrectly formatted!: ", line), true);
			return false;
		}
		if (camera.fov < 0 || camera.fov > 180)
		{
			set_error(ft_strjoin("Camera FOV out of range! (0-180): ", line), true);
			return false;
		}
		darray_push(&scene->cameras, &camera);
	}
	else if (line[0] == 'l')
	{
		skip_whitespace(line, &curr);
		t_light light;
		if (!read_vec3(line, &curr, &light.position))
		{
			set_error(ft_strjoin("Light position incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, &light.color))
		{
			set_error(ft_strjoin("Light color incorrectly formatted: ", line), true);
			return false;
		}
	}
	else {
		if (line[0] == '\0')
			return (true);
		set_error(ft_strjoin("Unknown configuration: ", line), true);
		return false;
	}

	skip_whitespace(line, &curr);
	if (line[curr] != '\0')
	{
		set_error(ft_strjoin("Line contains more data than expected: ", line), true);
		return false;
	}
	return true;
}

t_scene* parse_scene_file(int fd)
{
	t_scene* scene = malloc(sizeof(t_scene));
	if (scene == NULL)
	{
		set_error("Malloc failed in parse_scene_file!", false);
		return NULL;
	}
	bool init_success = true;
	init_success = darray_init(&scene->cameras, sizeof(t_camera)) && init_success;
	init_success = darray_init(&scene->objects, sizeof(t_object)) && init_success;
	init_success = darray_init(&scene->lights, sizeof(t_light)) && init_success;
	if (!init_success)
	{
		darray_free(&scene->cameras);
		darray_free(&scene->objects);
		darray_free(&scene->lights);
		free(scene);
		set_error("Malloc failed in darray init!", false);
	}
	char* line;

	t_scene_parse_data parse_data;
	parse_data.has_ambiant = false;
	parse_data.has_resolution = false;
	while (true)
	{
		int out = get_next_line(fd, &line);
		if (out == -1)
		{
			set_error(ft_strjoin("Could not read file: ", strerror(errno)), true);
			free(scene);
			return NULL;
		}

		if (!parse_line(&parse_data, scene, line))
		{
			free(line);
			free(scene);
			return NULL;
		}
		free(line);

		if (out == 0)
			break;
	}

	if (!parse_data.has_ambiant || !parse_data.has_resolution || scene->cameras.count == 0)
	{
		darray_free(&scene->cameras);
		darray_free(&scene->objects);
		free(scene);
		if (!parse_data.has_ambiant)
			set_error("No ambiant in configuration!", false);
		else if (!parse_data.has_resolution)
			set_error("No resolution in configuration!", false);
		else if (scene->cameras.count == 0)
			set_error("No cameras in configuration", false);
		return NULL;
	}

	return scene;
}