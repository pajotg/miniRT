/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_sphere.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:57 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 15:24:56 by jsimonis      ########   odam.nl         */
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

bool ray_intersects_sphere(const t_object* object, const t_ray* ray, t_ray_hit* o_hit)
{
	t_object_sphere* data = object->object_data;
	t_vec3 offset;
	vec3_subtract(&offset, &object->transform.position, &ray->origin);

	float dot = vec3_dot(&ray->direction,&offset);
	float delta = dot * dot - (vec3_magnitude_sqr(&offset) - data->radius * data->radius);

	if (delta < 0)
		return false;

	delta = sqrtf(delta);
	bool is_inside = delta > dot;

	// Distance is either dot - delta or dot + delta, if we can subtract, subtract
	if (delta < dot)
		delta = -delta;
	float distance = dot + delta;
	if (o_hit->distance < distance || distance < 0)
		return false;
	o_hit->distance = distance;
	o_hit->object = (t_object*)object;

	vec3_scale(&o_hit->location, &ray->direction, o_hit->distance);
	vec3_add(&o_hit->location, &ray->origin, &o_hit->location);

	vec3_subtract(&o_hit->normal, &o_hit->location, &object->transform.position);
	vec3_scale(&o_hit->normal, &o_hit->normal, 1 / data->radius);

	if (is_inside)
		o_hit->normal = (t_vec3) { -o_hit->normal.x, -o_hit->normal.y, -o_hit->normal.z };
	return true;
}
