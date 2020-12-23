/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   object_intersections.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:14:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 17:36:44 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_objects.h"
#include <stdbool.h>

bool ray_intersects_sphere(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_sphere* data = object->object_data;
	(void)data;
	(void)object;
	(void)ray;
	(void)hit;
	return false;
}

bool ray_intersects_plane(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_plane* data = object->object_data;

	t_vec3 normal = quaternion_mult_vec3(object->transform.rotation, vec3_new(0, 0, -1));
	float height = vec3_dot( normal, vec3_subtract(ray->origin, object->transform.position ) );
	float travel_distance = height / vec3_dot( normal, ray->direction );

	if (travel_distance < 0)
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