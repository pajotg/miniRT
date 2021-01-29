/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cast_ray.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 14:52:36 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/29 16:49:18 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_scene.h"
#include "mini_rt_object.h"
#include "mini_rt_ray_intersects.h"
#include <stdbool.h>
#include <math.h>

static bool cast_ray_raw(const t_scene* scene, const t_ray* ray, t_ray_hit* o_hit)
{
	bool has_hit = false;
	for (size_t i = 0; i < scene->objects.count; i++)
	{
		t_object* obj = list_index(&scene->objects, i);
		if (ray_intersects_aabb(ray, &obj->aabb, o_hit->distance) && obj->intersect_func(obj, ray, o_hit))
			has_hit = true;
	}
	return has_hit;
}

bool cast_ray(const t_scene* scene, const t_ray* ray, t_ray_hit* o_hit)
{
	o_hit->distance = INFINITY;
	return cast_ray_raw(scene, ray, o_hit);
}

bool cast_ray_max(const t_scene* scene, const t_ray* ray, t_ray_hit* o_hit, float max_dist)
{
	o_hit->distance = max_dist;
	return cast_ray_raw(scene, ray, o_hit);
}