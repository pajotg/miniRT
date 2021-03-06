/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_square.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:59:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/11 21:57:53 by jsimonis      ########   odam.nl         */
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
// read extends

static void	calculate_aabb_stupid_norm(t_object *object, t_object_square *
	square)
{
	object->aabb.max = (t_vec3){square->extends, square->extends, 0 };
	object->aabb.min = (t_vec3){-square->extends, -square->extends, 0 };
	vec3_add(&object->aabb.min, &object->aabb.min, &object->transform.position);
	vec3_add(&object->aabb.max, &object->aabb.max, &object->transform.position);
	aabb_rotate(&object->aabb, &object->transform.rotation);
}

static bool	stupid_norm_1(t_object *object, t_object_square	*square, char *line,
	int *curr)
{
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &object->transform))
	{
		free(square);
		set_error(ft_strjoin(
				"square position and normal incorrectly formatted: ", line),
			true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_float(line, curr, &square->extends))
	{
		free(square);
		set_error(ft_strjoin(
				"square size incorrectly formatted: ", line), true);
		return (false);
	}
	square->extends /= 2;
	return (true);
}

static bool	stupid_norm_2(t_object *object, t_object_square	*square, char *line,
	int *curr)
{
	skip_whitespace(line, curr);
	object->material = read_material(line, curr);
	if (!object->material)
	{
		free(square);
		set_error(ft_strjoin_va(4, "square material incorrectly formatted: ",
				line, "\nReason: ", get_last_error()), true);
		return (false);
	}
	return (true);
}

bool	scene_parse_square(t_scene *scene, char *line, int *curr)
{
	t_object		object;
	t_object_square	*square;

	square = malloc(sizeof(t_object_square));
	if (!square)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	object.object_data = square;
	object.intersect_func = (t_object_intersect_func)ray_intersects_square;
	if (!stupid_norm_1(&object, square, line, curr) || !stupid_norm_2(&object,
			square, line, curr))
		return (false);
	calculate_aabb_stupid_norm(&object, square);
	if (!list_push(&scene->objects, &object))
	{
		free(square);
		shared_pt_release_and_free(object.material);
		set_error("Could not push square into objects list!", true);
		return (false);
	}
	return (true);
}
