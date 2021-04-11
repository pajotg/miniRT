/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_triangle.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 17:08:45 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/11 21:58:03 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"
//#include "mini_rt_object.h"
#include "ft_error.h"
#include <stdlib.h>
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"
//#include "mini_rt_material_data.h"

// Calculate the aabb
// Read material

static void	calculate_aabb_stupid_norm(t_object *object, t_object_triangle *
	triangle)
{
	aabb_init(&object->aabb, &object->transform.position);
	aabb_extend_point(&object->aabb, &triangle->second_point);
	aabb_extend_point(&object->aabb, &triangle->third_point);
}

static bool	stupid_norm_1(t_object *object, t_object_triangle *triangle, char *
	line,
	int *curr)
{
	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, ',', &object->transform.position))
	{
		free(triangle);
		set_error(ft_strjoin(
				"triangle first position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, ',', &triangle->second_point))
	{
		free(triangle);
		set_error(ft_strjoin(
				"triangle second position incorrectly formatted: ", line), true)
		;
		return (false);
	}
	return (true);
}

static bool	stupid_norm_2(t_object *object, t_object_triangle *triangle, char *
	line,
	int *curr)
{
	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, ',', &triangle->third_point))
	{
		free(triangle);
		set_error(ft_strjoin(
				"triangle third position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	object->material = read_material(line, curr);
	if (!object->material)
	{
		free(triangle);
		set_error(ft_strjoin_va(4, "triangle material incorrectly formatted: ",
				line, "\nReason: ", get_last_error()), true);
		return (false);
	}
	return (true);
}

bool	scene_parse_triangle(t_scene *scene, char *line, int *curr)
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
	if (!stupid_norm_1(&object, triangle, line, curr) || !stupid_norm_2(&object,
			triangle, line, curr))
		return (false);
	calculate_aabb_stupid_norm(&object, triangle);
	if (!list_push(&scene->objects, &object))
	{
		free(triangle);
		shared_pt_release_and_free(object.material);
		set_error("Could not push triangle into objects list!", true);
		return (false);
	}
	return (true);
}
