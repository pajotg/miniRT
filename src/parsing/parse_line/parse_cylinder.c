/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 17:05:43 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/28 15:49:08 by jsimonis      ########   odam.nl         */
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


	// Read radius
	if (!read_float(line, curr, &cylinder->radius))
	{
		free(cylinder);
		set_error(ft_strjoin(
			"cylinder diameter incorrectly formatted: ", line), true);
		return (false);
	}
	cylinder->radius /= 2;

	// Read extends
	skip_whitespace(line, curr);
	if (!read_float(line, curr, &cylinder->height_extends))
	{
		free(cylinder);
		set_error(ft_strjoin(
			"cylinder height incorrectly formatted: ", line), true);
		return (false);
	}
	cylinder->height_extends /= 2;

	// Read material
	skip_whitespace(line, curr);
	t_color_hdr color;
	if (!read_color(line, curr, false, &color))
	{
		free(cylinder);
		set_error(ft_strjoin(
			"sphere color incorrectly formatted: ", line), true);
		return (false);
	}
	object.material = material_diffuse_new(&color);
	if (object.material == NULL)
	{
		free(cylinder);
		set_error(ft_strjoin(
			"Failed to init diffuse material! ", line), true);
		return (false);
	}

	// Calculate the aabb
	object.aabb.max = (t_vec3) {  cylinder->radius,  cylinder->height_extends,  cylinder->radius };
	object.aabb.min = (t_vec3) { -cylinder->radius, -cylinder->height_extends, -cylinder->radius };
	vec3_add(&object.aabb.min, &object.aabb.min, &object.transform.position);
	vec3_add(&object.aabb.max, &object.aabb.max, &object.transform.position);
	aabb_rotate(&object.aabb,&object.transform.rotation);
	if (!list_push(&scene->objects, &object))
	{
		set_error("Could not push cylinder into objects list!", true);
		return (false);
	}
	return (true);
}
