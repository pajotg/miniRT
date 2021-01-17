/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_triangle.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 17:08:45 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/17 13:22:58 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include "mini_rt_objects.h"
#include <stdlib.h>
#include "ft_error.h"

static bool	parse_triangle_ext(t_object_triangle *triangle,
	char *line, int *curr)
{
	if (!read_vec3(line, curr, &triangle->second_point))
	{
		free(triangle);
		set_error(ft_strjoin(
			"triangle second position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, &triangle->third_point))
	{
		free(triangle);
		set_error(ft_strjoin(
			"triangle third position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_color(line, curr, false, &triangle->color))
	{
		free(triangle);
		set_error(ft_strjoin(
			"triangle color incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}

bool		parse_triangle(t_scene *scene, char *line, int *curr)
{
	t_object			object;
	t_object_triangle	*triangle;

	triangle = malloc(sizeof(t_object_triangle));
	if (!triangle)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = triangle;
	object.intersect_func = (t_object_intersect_func)ray_intersects_triangle;
	object.transform.rotation = *quaternion_identity();
	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, &object.transform.position))
	{
		free(triangle);
		set_error(ft_strjoin(
			"triangle first position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!parse_triangle_ext(triangle, line, curr))
		return (false);
	// Calculate the aabb
	aabb_init(&object.aabb, &object.transform.position);
	aabb_extend_point(&object.aabb, &triangle->second_point);
	aabb_extend_point(&object.aabb, &triangle->third_point);
	if (!list_push(&scene->objects, &object))
	{
		set_error("Could not push triangle into objects list!", true);
		return (false);
	}
	return (true);
}
