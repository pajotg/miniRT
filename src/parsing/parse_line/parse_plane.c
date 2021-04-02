/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_plane.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:47:01 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/02 16:13:29 by jsimonis      ########   odam.nl         */
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

// Calculate the aabb

static bool	stupid_norm(t_object *object, t_object_plane *plane, char *line,
	int *curr)
{
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object->transform))
	{
		free(plane);
		set_error(ft_strjoin(
				"plane position and normal incorrectly formatted: ", line),
			true);
		return (false);
	}
	skip_whitespace(line, curr);
	object->material = read_material(line, curr);
	if (!object->material)
	{
		free(plane);
		set_error(ft_strjoin_va(4, "plane material incorrectly formatted: ",
				line, "\nReason: ", get_last_error()), true);
		return (false);
	}
	return (true);
}

bool	scene_parse_plane(t_scene *scene, char *line, int *curr)
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
	if (!stupid_norm(&object, plane, line, curr))
		return (false);
	object.aabb.max = (t_vec3){INFINITY, INFINITY, INFINITY};
	object.aabb.min = (t_vec3){-INFINITY, -INFINITY, -INFINITY};
	if (!list_push(&scene->objects, &object))
	{
		free(plane);
		shared_pt_release_and_free(object.material);
		set_error("Could not push plane into objects list!", true);
		return (false);
	}
	return (true);
}
