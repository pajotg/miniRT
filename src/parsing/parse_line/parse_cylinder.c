/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 17:05:43 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 11:40:10 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include "mini_rt_objects.h"
#include <stdlib.h>

static bool	parse_cylinder_ext(t_object_cylinder *cylinder,
	char *line, int *curr)
{
	if (!read_float(line, curr, &cylinder->radius))
	{
		free(cylinder);
		set_error(ft_strjoin(
			"cylinder diameter incorrectly formatted: ", line), true);
		return (false);
	}
	cylinder->radius /= 2;
	skip_whitespace(line, curr);
	if (!read_float(line, curr, &cylinder->height))
	{
		free(cylinder);
		set_error(ft_strjoin(
			"cylinder height incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_color(line, curr, false, &cylinder->color))
	{
		free(cylinder);
		set_error(ft_strjoin(
			"cylinder color incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}

bool		parse_cylinder(t_scene *scene, char *line, int *curr)
{
	t_object			object;
	t_object_cylinder	*cylinder;

	cylinder = malloc(sizeof(t_object_cylinder));
	if (!cylinder)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = cylinder;
	object.intersect_func = (t_object_intersect_func)ray_intersects_cylinder;
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object.transform))
	{
		free(cylinder);
		set_error(ft_strjoin(
"cylinder position and normal incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!parse_cylinder_ext(cylinder, line, curr))
		return (false);
	darray_push(&scene->objects, &object);
	return (true);
}
