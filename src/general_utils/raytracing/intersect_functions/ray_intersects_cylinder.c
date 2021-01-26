/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_cylinder.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:25 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:51:49 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "mini_rt_object_data.h"
#include <math.h>

/*
**	Woah, that took FOREVER, so manny mistakes
**	https://math.stackexchange.com/questions/2613781/line-cylinder-intersection
*/

bool ray_intersects_cylinder_top_and_bottom(float Height, float Radius, t_vec3* origin, t_vec3* dir, t_ray_hit* hit)
{
	bool Above = (dir->z < 0) != (origin->z < Height && origin->z > -Height);

	float Plane_z = Above ? Height : -Height;
	float Dist = (Plane_z - origin->z) / dir->z;
	if (Dist > hit->distance)
		return (false);

	float OutX = origin->x + dir->x * Dist;
	float OutY = origin->y + dir->y * Dist;

	float SqrMag = OutX * OutX + OutY * OutY;
	if (SqrMag > Radius * Radius)
		return (false);

	hit->distance = Dist;
	vec3_scale(&hit->location, dir, Dist);
	vec3_add(&hit->location, &hit->location, origin);
	hit->normal = (t_vec3) { 0, 0, dir->z > 0 ? -1 : 1 };

	return (true);
}

bool ray_intersects_cylinder(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_cylinder* data = object->object_data;

	t_quaternion conj;
	t_vec3 origin;
	t_vec3 dir;
	quaternion_conjugate(&conj, &object->transform.rotation);
	vec3_subtract(&origin, &ray->origin, &object->transform.position);
	quaternion_mult_vec3(&origin, &conj, &origin);
	quaternion_mult_vec3(&dir, &conj, &ray->direction);

	// If we are above or below, first check top/bottom
	if (origin.x * origin.x + origin.y * origin.y < data->radius * data->radius)
	{
		if (ray_intersects_cylinder_top_and_bottom(data->height_extends, data->radius, &origin, &dir, hit))
		{
			hit->color = data->color;
			quaternion_mult_vec3(&hit->location, &object->transform.rotation, &hit->location);
			quaternion_mult_vec3(&hit->normal, &object->transform.rotation, &hit->normal);
			vec3_add(&hit->location, &hit->location, &object->transform.position);
			return (true);
		}
		if (origin.z > data->height_extends || origin.z < -data->height_extends)
			return (false);	// We can only return false if we are not inside the cylinder
	}

	float dir2d_sqr = dir.x * dir.x + dir.y * dir.y;
	float t = - (origin.x * dir.x + dir.y * origin.y) / dir2d_sqr;
	float dsqr = origin.x * origin.x + origin.y * origin.y - t * t * dir2d_sqr;
	if (dsqr > data->radius * data->radius)
		return (false);
	float k = sqrtf((data->radius * data->radius - dsqr) / dir2d_sqr);

	bool is_inside = t < k;
	if (is_inside)
		k = -k;
	float dist = t - k;
	if (dist > hit->distance || dist < 0)
		return (false);

	// If we are above or below the height, raycast to the top and bottom
	float out_z = origin.z + dir.z * dist;
	if (out_z > data->height_extends || out_z < -data->height_extends)
	{
		if (ray_intersects_cylinder_top_and_bottom(data->height_extends, data->radius, &origin, &dir, hit))
		{
			hit->color = data->color;
			quaternion_mult_vec3(&hit->location, &object->transform.rotation, &hit->location);
			quaternion_mult_vec3(&hit->normal, &object->transform.rotation, &hit->normal);
			vec3_add(&hit->location, &hit->location, &object->transform.position);
			return (true);
		}
		return (false);
	}

	hit->distance = dist;
	hit->color = data->color;

	vec3_scale(&hit->location, &ray->direction, dist);
	vec3_add(&hit->location, &hit->location, &ray->origin);

	hit->normal = (t_vec3) {
		(origin.x + dir.x * dist) / data->radius,
		(origin.y + dir.y * dist) / data->radius,
		0
	};
	if (is_inside)
	{
		hit->normal.x = -hit->normal.x;
		hit->normal.y = -hit->normal.y;
	}
	quaternion_mult_vec3(&hit->normal, &object->transform.rotation, &hit->normal);
	return (true);
}
