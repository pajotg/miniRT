/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_sphere.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:39:05 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/02 16:28:19 by jsimonis      ########   odam.nl         */
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

// Calculate the aabb

static bool	stupid_norm(t_object *object, t_object_sphere *sphere, char *line,
	int *curr)
{
	if (!read_vec3(line, curr, ',', &object->transform.position))
	{
		free(sphere);
		set_error(ft_strjoin(
				"sphere position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_float(line, curr, &sphere->radius))
	{
		free(sphere);
		set_error(ft_strjoin("sphere missing diameter: ", line), true);
		return (false);
	}
	sphere->radius /= 2;
	skip_whitespace(line, curr);
	object->material = read_material(line, curr);
	if (!object->material)
	{
		free(sphere);
		set_error(ft_strjoin_va(4, "sphere material incorrectly formatted: ",
				line, "\nReason: ", get_last_error()), true);
		return (false);
	}
	return (true);
}

static void	stupid_norm2(t_object *object, t_object_sphere *sphere)
{
	object->aabb.max = (t_vec3){sphere->radius, sphere->radius, sphere->radius};
	object->aabb.min = (t_vec3){-sphere->radius, -sphere->radius, -sphere
		->radius};
	vec3_add(&object->aabb.min, &object->aabb.min, &object->transform.position);
	vec3_add(&object->aabb.max, &object->aabb.max, &object->transform.position);
}

bool	scene_parse_sphere(t_scene *scene, char *line, int *curr)
{
	t_object		object;
	t_object_sphere	*sphere;

	sphere = malloc(sizeof(t_object_sphere));
	if (!sphere)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = sphere;
	object.intersect_func = (t_object_intersect_func)ray_intersects_sphere;
	skip_whitespace(line, curr);
	object.transform.rotation = *quaternion_identity();
	if (!stupid_norm(&object, sphere, line, curr))
		return (false);
	stupid_norm2(&object, sphere);
	if (!list_push(&scene->objects, &object))
	{
		free(sphere);
		shared_pt_release_and_free(object.material);
		set_error("Could not push sphere into objects list!", true);
		return (false);
	}
	return (true);
}
