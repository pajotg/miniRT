/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cube.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:59:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 13:08:17 by jsimonis      ########   odam.nl         */
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

bool		scene_parse_cube(t_scene *scene, char *line, int *curr)
{
	t_object		object;
	t_object_cube	*cube;

	cube = malloc(sizeof(t_object_cube));
	if (!cube)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = cube;
	object.intersect_func = (t_object_intersect_func)ray_intersects_cube;

	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object.transform))
	{
		free(cube);
		set_error(ft_strjoin(
			"cube position and normal incorrectly formatted: ", line), true);
		return (false);
	}

	skip_whitespace(line, curr);
	if (!read_float(line, curr, &cube->extends))
	{
		free(cube);
		set_error(ft_strjoin(
			"cube size incorrectly formatted: ", line), true);
		return (false);
	}
	cube->extends /= 2;

	skip_whitespace(line, curr);
	object.material = read_material(line, curr);
	if (!object.material)
	{
		free(cube);
		set_error(ft_strjoin_va(4, "cube material incorrectly formatted: ", line, "\nReason: ", get_last_error()), true);
		return (false);
	}

	// Calculate the aabb
	object.aabb.max = (t_vec3) {  cube->extends,  cube->extends,  cube->extends };
	object.aabb.min = (t_vec3) { -cube->extends, -cube->extends, -cube->extends };
	vec3_add(&object.aabb.min, &object.aabb.min, &object.transform.position);
	vec3_add(&object.aabb.max, &object.aabb.max, &object.transform.position);
	aabb_rotate(&object.aabb,&object.transform.rotation);

	if (!list_push(&scene->objects, &object))
	{
		free(cube);
		shared_pt_release_and_free(object.material);
		set_error("Could not push cube into objects list!", true);
		return (false);
	}
	return (true);
}
