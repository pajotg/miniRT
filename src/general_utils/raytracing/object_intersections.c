/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   object_intersections.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:14:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/25 10:43:44 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_objects.h"
#include "mini_rt_color_math_utils.h"
#include <stdbool.h>

#include <stdio.h>	// bad

/*
**	https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
*/

/*
**	Skipping case of delta = 0, when will that ever appear?
**	delta > -dot should make it work from inside the sphere too
*/

bool ray_intersects_sphere(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_sphere* data = object->object_data;
	t_vec3 offset = vec3_subtract(object->transform.position, ray->origin);

	float dot = vec3_dot(ray->direction,offset);
	float delta = dot * dot - (vec3_magnitude_sqr(offset) - data->radius * data->radius);

	if (delta < 0)
		return false;
	if (delta > dot)
		delta = -delta;
	float distance = dot - delta;
	if (hit->distance < distance)
		return false;
	hit->distance = distance;
	hit->color = data->color;
	hit->location = vec3_add(ray->origin, vec3_scale(ray->direction, hit->distance));
	hit->normal = vec3_scale(vec3_subtract(hit->location, object->transform.position), 1 / data->radius);
	return true;
}

/*
**	Pure memory
*/

bool ray_intersects_plane(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_plane* data = object->object_data;

	t_vec3 normal = quaternion_mult_vec3(object->transform.rotation, vec3_new(0, 0, -1));
	float height = vec3_dot( normal, vec3_subtract(object->transform.position, ray->origin) );
	float travel_distance = height / vec3_dot( normal, ray->direction );

	if (travel_distance < 0 || travel_distance > hit->distance)
		return false;

	hit->distance = travel_distance;
	hit->color = data->color;
	hit->location = vec3_add(ray->origin, vec3_scale(ray->direction, travel_distance));
	hit->normal = normal;
	return true;
}

bool ray_intersects_square(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_square* data = object->object_data;
	(void)data;
	(void)object;
	(void)ray;
	(void)hit;
	return false;
}

bool ray_intersects_cylinder(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_cylinder* data = object->object_data;
	(void)data;
	(void)object;
	(void)ray;
	(void)hit;
	return false;
}

bool ray_intersects_triangle(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_triangle* data = object->object_data;
	(void)data;
	(void)object;
	(void)ray;
	(void)hit;
	return false;
}