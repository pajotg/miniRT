/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_sphere.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:57 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/29 17:02:09 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "mini_rt_object_data.h"
#include <math.h>

/*
**	https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
**	Skipping case of delta = 0, when will that ever appear?
**	delta > -dot should make it work from inside the sphere too
*/

static bool	calculate_hit_from_distance(const t_object *object, float distance,
	const t_ray *ray, t_ray_hit *o_hit)
{
	t_object_sphere	*data;

	data = object->object_data;
	if (o_hit->distance < distance || distance < 0)
		return (false);
	o_hit->distance = distance;
	o_hit->object = (t_object *)object;
	vec3_scale(&o_hit->location, &ray->direction, o_hit->distance);
	vec3_add(&o_hit->location, &ray->origin, &o_hit->location);
	vec3_subtract(&o_hit->normal, &o_hit->location, &object->transform.position)
	;
	vec3_scale(&o_hit->normal, &o_hit->normal, 1 / data->radius);
	return (true);
}

// Distance is either dot - delta or dot + delta, if we can subtract, subtract

bool	ray_intersects_sphere(const t_object *object, const t_ray *ray,
	t_ray_hit *o_hit)
{
	t_vec3			offset;
	float			dot;
	float			delta;
	bool			is_inside;
	t_object_sphere	*data;

	data = object->object_data;
	vec3_subtract(&offset, &object->transform.position, &ray->origin);
	dot = vec3_dot(&ray->direction, &offset);
	delta = dot * dot - (vec3_magnitude_sqr(&offset) - data->radius
			 * data->radius);
	if (delta < 0)
		return (false);
	delta = sqrtf(delta);
	is_inside = delta > dot;
	if (delta < dot)
		delta = -delta;
	if (!calculate_hit_from_distance(object, dot + delta, ray, o_hit))
		return (false);
	if (is_inside)
		vec3_negate(&o_hit->normal, &o_hit->normal);
	return (true);
}
