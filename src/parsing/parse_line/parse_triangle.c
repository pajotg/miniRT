/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_triangle.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 17:08:45 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/28 15:51:12 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"
#include "mini_rt_object.h"
#include "ft_error.h"
#include <stdlib.h>
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"
#include "mini_rt_material_data.h"

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

	// Read material
	skip_whitespace(line, curr);
	t_color_hdr color;
	if (!read_color(line, curr, false, &color))
	{
		free(triangle);
		set_error(ft_strjoin(
			"sphere color incorrectly formatted: ", line), true);
		return (false);
	}
	object.material = material_diffuse_new(&color);
	if (object.material == NULL)
	{
		free(triangle);
		set_error(ft_strjoin(
			"Failed to init diffuse material! ", line), true);
		return (false);
	}

	// Calculate the aabb
	aabb_init(&object.aabb, &object.transform.position);
	aabb_extend_point(&object.aabb, &triangle->second_point);
	aabb_extend_point(&object.aabb, &triangle->third_point);
	if (!list_push(&scene->objects, &object))
	{
		free(triangle);
		material_free(object.material);
		set_error("Could not push triangle into objects list!", true);
		return (false);
	}
	return (true);
}
