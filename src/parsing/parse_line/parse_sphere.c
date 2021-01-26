/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_sphere.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:39:05 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 18:37:40 by jsimonis      ########   odam.nl         */
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

static bool	parse_sphere_extension(t_object_sphere *sphere,
	char *line, int *curr)
{
	if (!read_float(line, curr, &sphere->radius))
	{
		free(sphere);
		set_error(ft_strjoin("sphere missing diameter: ", line), true);
		return (false);
	}
	sphere->radius /= 2;
	skip_whitespace(line, curr);
	if (!read_color(line, curr, false, &sphere->color))
	{
		free(sphere);
		set_error(ft_strjoin(
			"sphere color incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}

bool		parse_sphere(t_scene *scene, char *line, int *curr)
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
	if (!read_vec3(line, curr, &object.transform.position))
	{
		free(sphere);
		set_error(ft_strjoin(
			"sphere position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!parse_sphere_extension(sphere, line, curr))
		return (false);
	// Calculate the aabb
	object.aabb.max = (t_vec3) {  sphere->radius,  sphere->radius,  sphere->radius };
	object.aabb.min = (t_vec3) { -sphere->radius, -sphere->radius, -sphere->radius };
	vec3_add(&object.aabb.min, &object.aabb.min, &object.transform.position);
	vec3_add(&object.aabb.max, &object.aabb.max, &object.transform.position);
	if (!list_push(&scene->objects, &object))
	{
		set_error("Could not push sphere into objects list!", true);
		return (false);
	}
	return (true);
}
