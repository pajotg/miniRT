/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cast_ray.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 14:52:36 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:45:34 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//#include "mini_rt_scene.h"
//#include "mini_rt_object.h"
#include "mini_rt_ray_intersects.h"
//#include <stdbool.h>
#include <math.h>

static bool	cast_ray_bvh(const t_bvh *bvh, const t_ray *ray, t_ray_hit *o_hit)
{
	t_object	*obj;
	bool		has_hit;

	if (!ray_intersects_aabb(ray, &bvh->aabb, o_hit->distance))
		return (false);
	has_hit = false;
	if (bvh->is_leaf)
	{
		obj = bvh->a;
		if (obj->intersect_func(obj, ray, o_hit))
			has_hit = true;
		obj = bvh->b;
		if (obj && obj->intersect_func(obj, ray, o_hit))
			has_hit = true;
	}
	else
	{
		has_hit |= cast_ray_bvh(bvh->a, ray, o_hit);
		has_hit |= cast_ray_bvh(bvh->b, ray, o_hit);
	}
	return (has_hit);
}

static bool	cast_ray_scene(const t_scene *scene, const t_ray *ray,
	t_ray_hit *o_hit)
{
	size_t		i;
	bool		has_hit;
	t_object	*obj;

	has_hit = false;
	i = 0;
	while (i < scene->objects.count)
	{
		obj = list_index(&scene->objects, i);
		if (ray_intersects_aabb(ray, &obj->aabb, o_hit->distance)
			&& obj->intersect_func(obj, ray, o_hit))
			has_hit = true;
		i++;
	}
	return (has_hit);
}

static bool	cast_ray_raw(const t_scene *scene, const t_ray *ray,
	t_ray_hit *o_hit)
{
	if (scene->bvh)
		return (cast_ray_bvh(scene->bvh, ray, o_hit));
	return (cast_ray_scene(scene, ray, o_hit));
}

bool	cast_ray(const t_scene *scene, const t_ray *ray, t_ray_hit *o_hit)
{
	o_hit->distance = INFINITY;
	return (cast_ray_raw(scene, ray, o_hit));
}

bool	cast_ray_max(const t_scene *scene, const t_ray *ray, t_ray_hit *o_hit,
	float max_dist)
{
	o_hit->distance = max_dist;
	return (cast_ray_raw(scene, ray, o_hit));
}
