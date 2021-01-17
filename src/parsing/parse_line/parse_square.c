/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_square.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:59:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/17 13:48:14 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include "mini_rt_objects.h"
#include <stdlib.h>
#include "ft_error.h"

static bool	parse_square_ext(t_object_square *square, char *line, int *curr)
{
	if (!read_float(line, curr, &square->extends))
	{
		free(square);
		set_error(ft_strjoin(
			"square size incorrectly formatted: ", line), true);
		return (false);
	}
	square->extends /= 2;
	skip_whitespace(line, curr);
	if (!read_color(line, curr, false, &square->color))
	{
		free(square);
		set_error(ft_strjoin(
			"square color incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}

bool		parse_square(t_scene *scene, char *line, int *curr)
{
	t_object		object;
	t_object_square *square;

	square = malloc(sizeof(t_object_square));
	if (!square)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = square;
	object.intersect_func = (t_object_intersect_func)ray_intersects_square;
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object.transform))
	{
		free(square);
		set_error(ft_strjoin(
			"square position and normal incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!parse_square_ext(square, line, curr))
		return (false);
	// Calculate the aabb
	object.aabb.max = (t_vec3) {  square->extends,  square->extends, 0 };
	object.aabb.min = (t_vec3) { -square->extends, -square->extends, 0 };
	vec3_add(&object.aabb.min, &object.aabb.min, &object.transform.position);
	vec3_add(&object.aabb.max, &object.aabb.max, &object.transform.position);
	aabb_rotate(&object.aabb,&object.transform.rotation);
	if (!list_push(&scene->objects, &object))
	{
		set_error("Could not push square into objects list!", true);
		return (false);
	}
	return (true);
}
