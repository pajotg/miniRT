/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_file.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 19:27:40 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/27 17:11:52 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include <stddef.h>
#include "mini_rt_utils.h"
#include "mini_rt_objects.h"
#include "get_next_line.h"
#include <string.h>
#include "libft.h"
#include <math.h>
#include <errno.h>

#include <stdio.h>	// bad

static bool is_object(char *line, char *object, int *curr)
{
	int i;

	i = *curr;
	while (*object)
	{
		if (line[i] != *object)
			return (false);
		object++;
		i++;
	}
	if (!ft_isspace(line[i]))
		return (false);
	*curr = i + 1;
	return (true);
}

static bool parse_line(t_scene_parse_data* parse_data, t_scene* scene, char* line)
{
	int curr = 0;
	bool success = true;

	if (is_object(line, "R", &curr))
		success = parse_resolution(parse_data, scene, line, &curr);
	else if (is_object(line, "A", &curr))
		success = parse_ambiant(parse_data, scene, line, &curr);
	else if (is_object(line, "c", &curr))
		success = parse_camera(scene, line, &curr);
	else if (is_object(line, "l", &curr))
		success = parse_light(scene, line, &curr);
	else if (is_object(line, "sp", &curr))
		success = parse_sphere(scene, line, &curr);
	else if (is_object(line, "pl", &curr))
		success = parse_plane(scene, line, &curr);
	else if (is_object(line, "sq", &curr))
		success = parse_square(scene, line, &curr);
	else if (is_object(line, "cy", &curr))
		success = parse_cylinder(scene, line, &curr);
	else if (is_object(line, "tr", &curr))
		success = parse_triangle(scene, line, &curr);
	else {
		if (line[0] == '\0' || line[0] == '#')
			return (true);
		set_error(ft_strjoin("Unknown configuration: ", line), true);
		return false;
	}
	if (!success)
		return (false);

	skip_whitespace(line, &curr);
	if (line[curr] != '\0')
	{
		set_error(ft_strjoin("Line contains more data than expected: ", line), true);
		return false;
	}
	return true;
}

void un_init_object(t_object* object)
{
	free(object->object_data);
}

void free_scene(t_scene* scene)
{
	darray_un_init(&(scene->cameras), NULL);
	darray_un_init(&(scene->objects), (t_free_values)un_init_object);
	darray_un_init(&(scene->lights), NULL);
	free(scene);
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
		free_scene(scene);
		set_error("Malloc failed in darray init!", false);
		return NULL;
	}
	char* line;
	scene->current_camera_index = 0;

	t_scene_parse_data parse_data;
	parse_data.has_ambiant = false;
	parse_data.has_resolution = false;
	while (true)
	{
		int out = get_next_line(fd, &line);
		if (out == -1)
		{
			set_error(ft_strjoin("Could not read file: ", strerror(errno)), true);
			free_scene(scene);
			return NULL;
		}

		if (!parse_line(&parse_data, scene, line))
		{
			clear_gnl_data(fd);
			free(line);
			free_scene(scene);
			return NULL;
		}
		free(line);

		if (out == 0)
			break;
	}

	if (!parse_data.has_ambiant || !parse_data.has_resolution || scene->cameras.count == 0)
	{
		if (!parse_data.has_ambiant)
			set_error("No ambiant in configuration!", false);
		else if (!parse_data.has_resolution)
			set_error("No resolution in configuration!", false);
		else if (scene->cameras.count == 0)
			set_error("No cameras in configuration", false);
		free_scene(scene);
		return NULL;
	}

	return scene;
}