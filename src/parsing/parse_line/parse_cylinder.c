/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 17:05:43 by jasper        #+#    #+#                 */
/*   Updated: 2021/02/05 13:48:30 by jsimonis      ########   odam.nl         */
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

bool		scene_parse_cylinder(t_scene *scene, char *line, int *curr)
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
	object.material = read_material(line, curr);
	if (!object.material)
	{
		free(cylinder);
		set_error(ft_strjoin_va(4, "cylinder material incorrectly formatted: ", line, "\nReason: ", get_last_error()), true);
		return (false);
	}

	// Calculate the aabb
	object.aabb.max = (t_vec3) {  cylinder->radius,  cylinder->radius,  cylinder->height_extends };
	object.aabb.min = (t_vec3) { -cylinder->radius, -cylinder->radius, -cylinder->height_extends };
	vec3_add(&object.aabb.min, &object.aabb.min, &object.transform.position);
	vec3_add(&object.aabb.max, &object.aabb.max, &object.transform.position);
	aabb_rotate(&object.aabb,&object.transform.rotation);
	if (!list_push(&scene->objects, &object))
	{
		free(cylinder);
		shared_pt_release_and_free(object.material);
		set_error("Could not push cylinder into objects list!", true);
		return (false);
	}
	return (true);
}
