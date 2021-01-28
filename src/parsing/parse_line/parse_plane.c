/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_plane.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:47:01 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/28 15:44:04 by jsimonis      ########   odam.nl         */
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
#include <math.h>
#include "mini_rt_material_data.h"

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

	t_color_hdr color;
	if (!read_color(line, curr, false, &color))
	{
		free(plane);
		set_error(ft_strjoin(
			"plane color incorrectly formatted: ", line), true);
		return (false);
	}
	object.material = material_diffuse_new(&color);
	if (object.material == NULL)
	{
		free(plane);
		set_error(ft_strjoin(
			"Failed to init diffuse material! ", line), true);
		return (false);
	}

	// Calculate the aabb
	object.aabb.max = (t_vec3) { INFINITY, INFINITY, INFINITY };
	object.aabb.min = (t_vec3) { -INFINITY, -INFINITY, -INFINITY };
	if (!list_push(&scene->objects, &object))
	{
		free(plane);
		material_free(object.material);
		set_error("Could not push plane into objects list!", true);
		return (false);
	}
	return (true);
}
