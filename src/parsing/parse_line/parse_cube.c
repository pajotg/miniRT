/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cube.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:59:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 13:08:17 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include "mini_rt_objects.h"
#include "ft_error.h"
#include <stdlib.h>

static bool	parse_cube_ext(t_object_cube *cube, char *line, int *curr)
{
	if (!read_float(line, curr, &cube->size))
	{
		free(cube);
		set_error(ft_strjoin(
			"cube size incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_color(line, curr, false, &cube->color))
	{
		free(cube);
		set_error(ft_strjoin(
			"cube color incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}

bool		parse_cube(t_scene *scene, char *line, int *curr)
{
	t_object		object;
	t_object_cube *cube;

	cube = malloc(sizeof(t_object_cube));
	if (!cube)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = cube;
	object.intersect_func = (t_object_intersect_func)ray_intersects_cube;
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object.transform))
	{
		free(cube);
		set_error(ft_strjoin(
			"cube position and normal incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!parse_cube_ext(cube, line, curr))
		return (false);
	if (!list_push(&scene->objects, &object))
	{
		set_error("Could not push cube into objects list!", true);
		return (false);
	}
	return (true);
}
