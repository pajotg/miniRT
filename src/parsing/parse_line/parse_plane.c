/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_plane.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:47:01 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 11:40:07 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include "mini_rt_objects.h"
#include <stdlib.h>

static bool	parse_plane_ext(t_object_plane *plane, char *line, int *curr)
{
	if (!read_color(line, curr, false, &plane->color))
	{
		free(plane);
		set_error(ft_strjoin(
			"plane color incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}

bool		parse_plane(t_scene *scene, char *line, int *curr)
{
	t_object		object;
	t_object_plane	*plane;

	plane = malloc(sizeof(t_object_plane));
	if (!plane)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = plane;
	object.intersect_func = (t_object_intersect_func)ray_intersects_plane;
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object.transform))
	{
		free(plane);
		set_error(ft_strjoin(
			"plane position and normal incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!parse_plane_ext(plane, line, curr))
		return (false);
	darray_push(&scene->objects, &object);
	return (true);
}
