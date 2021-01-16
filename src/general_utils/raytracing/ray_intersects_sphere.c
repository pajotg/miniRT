/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_sphere.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:57 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/16 14:29:47 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_objects.h"
#include "mini_rt_color_math_utils.h"
#include <stdbool.h>
#include <math.h>

/*
**	https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
**	Skipping case of delta = 0, when will that ever appear?
**	delta > -dot should make it work from inside the sphere too
*/

bool ray_intersects_sphere(t_object* object, t_ray* ray, t_ray_hit* hit)
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
	if (hit->distance < distance || distance < 0)
		return false;
	hit->distance = distance;
	hit->color = data->color;

	vec3_scale(&hit->location, &ray->direction, hit->distance);
	vec3_add(&hit->location, &ray->origin, &hit->location);

	vec3_subtract(&hit->normal, &hit->location, &object->transform.position);
	vec3_scale(&hit->normal, &hit->normal, 1 / data->radius);

	if (is_inside)
		hit->normal = (t_vec3) { -hit->normal.x, -hit->normal.y, -hit->normal.z };
	return true;
}
