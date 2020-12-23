/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_file.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 19:27:40 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 17:08:24 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_utils.h"
#include <stddef.h>
#include "mini_rt_utils.h"
#include "mini_rt_objects.h"
#include "get_next_line.h"
#include <string.h>
#include <errno.h>
#include "libft.h"

#include <stdio.h>

bool parse_line(t_scene_parse_data* parse_data, t_scene* scene, char* line)
{
	int curr = 2;
	if (line[0] == 'R' && ft_isspace(line[1]))
	{
		if (parse_data->has_resolution)
		{
			set_error(ft_strjoin("Duplicate resolution: ", line), true);
			return false;
		}
		parse_data->has_resolution = true;
		skip_whitespace(line, &curr);
		if (!read_int(line, &curr, &scene->resolution.width))
		{
			set_error(ft_strjoin("resolution missing width: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_int(line, &curr, &scene->resolution.height))
		{
			set_error(ft_strjoin("resolution missing height: ", line), true);
			return false;
		}
	}
	else if (line[0] == 'A' && ft_isspace(line[1]))
	{
		if (parse_data->has_ambiant)
		{
			set_error(ft_strjoin("Duplicate ambiant: ", line), true);
			return false;
		}
		parse_data->has_ambiant = true;
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, true, &scene->ambiant))
		{
			set_error(ft_strjoin("Ambiant incorrectly formatted: ", line), true);
			return false;
		}
	}
	else if (line[0] == 'c' && ft_isspace(line[1]))
	{
		t_camera camera;
		skip_whitespace(line, &curr);
		if (!read_transform(line, &curr, &camera.transform))
		{
			set_error(ft_strjoin("Camera transform incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_float(line, &curr, &camera.fov))
		{
			set_error(ft_strjoin("Camera missing FOV: ", line), true);
			return false;
		}
		if (camera.fov < 0 || camera.fov > 180)
		{
			set_error(ft_strjoin("Camera FOV out of range! (0-180): ", line), true);
			return false;
		}
		darray_push(&scene->cameras, &camera);
	}
	else if (line[0] == 'l' && ft_isspace(line[1]))
	{
		skip_whitespace(line, &curr);
		t_light light;
		if (!read_vec3(line, &curr, &light.position))
		{
			set_error(ft_strjoin("Light position incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, true, &light.color))
		{
			set_error(ft_strjoin("Light color incorrectly formatted: ", line), true);
			return false;
		}
	}
	else if (ft_strncmp(line, "sp", 2) == 0)
	{
		t_object object;
		t_object_sphere* sphere = malloc(sizeof(t_object_sphere));
		if (!sphere)
		{
			set_error("Malloc failed", false);
			return false;
		}
		object.ObjectData = sphere;
		darray_push(&scene->objects, &object);
		skip_whitespace(line, &curr);
		if (!read_vec3(line, &curr, &object.Transform.position))
		{
			set_error(ft_strjoin("sphere position incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_float(line, &curr, &sphere->diameter))
		{
			set_error(ft_strjoin("sphere missing diameter: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, false, &sphere->color))
		{
			set_error(ft_strjoin("sphere color incorrectly formatted: ", line), true);
			return false;
		}
	}
	else if (ft_strncmp(line, "pl", 2) == 0)
	{
		t_object object;
		t_object_plane* plane = malloc(sizeof(t_object_plane));
		if (!plane)
		{
			set_error("Malloc failed", false);
			return false;
		}
		object.ObjectData = plane;
		darray_push(&scene->objects, &object);
		skip_whitespace(line, &curr);
		if (!read_transform(line, &curr, &object.Transform))
		{
			set_error(ft_strjoin("plane position and normal incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, false, &plane->color))
		{
			set_error(ft_strjoin("plane color incorrectly formatted: ", line), true);
			return false;
		}
	}
	else if (ft_strncmp(line, "sq", 2) == 0)
	{
		t_object object;
		t_object_square* square = malloc(sizeof(t_object_square));
		if (!square)
		{
			set_error("Malloc failed", false);
			return false;
		}
		object.ObjectData = square;
		darray_push(&scene->objects, &object);
		skip_whitespace(line, &curr);
		if (!read_transform(line, &curr, &object.Transform))
		{
			set_error(ft_strjoin("square position and normal incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_float(line, &curr, &square->size))
		{
			set_error(ft_strjoin("square size incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, false, &square->color))
		{
			set_error(ft_strjoin("square color incorrectly formatted: ", line), true);
			return false;
		}
	}
	else if (ft_strncmp(line, "cy", 2) == 0)
	{
		t_object object;
		t_object_cylinder* cylinder = malloc(sizeof(t_object_cylinder));
		if (!cylinder)
		{
			set_error("Malloc failed", false);
			return false;
		}
		object.ObjectData = cylinder;
		darray_push(&scene->objects, &object);
		skip_whitespace(line, &curr);
		if (!read_transform(line, &curr, &object.Transform))
		{
			set_error(ft_strjoin("cylinder position and normal incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_float(line, &curr, &cylinder->diameter))
		{
			set_error(ft_strjoin("cylinder diameter incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_float(line, &curr, &cylinder->height))
		{
			set_error(ft_strjoin("cylinder height incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, false, &cylinder->color))
		{
			set_error(ft_strjoin("cylinder color incorrectly formatted: ", line), true);
			return false;
		}
	}
	else if (ft_strncmp(line, "tr", 2) == 0)
	{
		t_object object;
		t_object_triangle* triangle = malloc(sizeof(t_object_triangle));
		if (!triangle)
		{
			set_error("Malloc failed", false);
			return false;
		}
		object.ObjectData = triangle;
		darray_push(&scene->objects, &object);
		skip_whitespace(line, &curr);
		if (!read_vec3(line, &curr, &object.Transform.position))
		{
			set_error(ft_strjoin("triangle first position incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_vec3(line, &curr, &triangle->second_point))
		{
			set_error(ft_strjoin("triangle second position incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_vec3(line, &curr, &triangle->third_point))
		{
			set_error(ft_strjoin("triangle third position incorrectly formatted: ", line), true);
			return false;
		}
		skip_whitespace(line, &curr);
		if (!read_color(line, &curr, false, &triangle->color))
		{
			set_error(ft_strjoin("triangle color incorrectly formatted: ", line), true);
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

void un_init_object(t_object* object)
{
	free(object->ObjectData);
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