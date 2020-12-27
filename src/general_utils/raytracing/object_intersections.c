/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   object_intersections.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:14:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/27 15:08:59 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_objects.h"
#include "mini_rt_color_math_utils.h"
#include <stdbool.h>
#include <math.h>

#include <stdio.h>	// bad
#include <stdlib.h> // bad
#include "ft_printf.h"	// bad

/*
**	BEWARE! you must not change the values of hit unless you actually have hit!
*/

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
	return true;
}

/*
**	Pure memory
*/

bool ray_intersects_plane(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_plane* data = object->object_data;
	t_vec3 normal;
	t_vec3 temp;

	quaternion_mult_vec3(&normal, &object->transform.rotation, vec3_forward());

	vec3_subtract(&temp, &ray->origin, &object->transform.position);
	float height = vec3_dot( &normal, &temp );
	float travel_distance = -height / vec3_dot( &normal, &ray->direction );

	if (travel_distance < 0 || travel_distance > hit->distance)
		return false;

	hit->distance = travel_distance;
	hit->color = data->color;

	vec3_scale(&temp, &ray->direction, travel_distance);
	vec3_add(&hit->location, &ray->origin, &temp);
	if (height > 0)
		hit->normal = normal;
	else
	{
		hit->normal.x = -normal.x;
		hit->normal.y = -normal.y;
		hit->normal.z = -normal.z;
	}
	return true;
}

/*
** Found this code, read through it, understood it, looks really good, does backface culling
** https://web.archive.org/web/20090803054252/http://tog.acm.org/resources/GraphicsGems/gems/RayBox.c
** Also learned about the register keyword, seems usefull, will use it more often
** I am pretty sure this works in any dimension too, so that is cool, maybe make a 4D raytracer? :)))
*/

bool ray_intersects_square(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_square* data = object->object_data;
	register int i;
	float min_b[3];
	float max_b[3];
	char side[3];
	float candidate_plane[3];
	bool inside = true;

	t_quaternion conj;
	t_vec3 origin_vec;
	t_vec3 dir_vec;
	quaternion_conjugate(&conj, &object->transform.rotation);
	vec3_subtract(&origin_vec, &ray->origin, &object->transform.position);
	quaternion_mult_vec3(&origin_vec, &conj, &origin_vec);
	quaternion_mult_vec3(&dir_vec, &conj, &ray->direction);

	float* origin = (float*)&origin_vec;
	float* dir = (float*)&dir_vec;

	for (i = 0; i < 3; i++)
	{
		min_b[i] = - data->size;
		max_b[i] = + data->size;
	}

	// Calculate which three planes we need to intersect
	for (i = 0; i < 3; i++)
	{
		if (origin[i] < min_b[i])
		{
			side[i] = -1;
			candidate_plane[i] = min_b[i];
			inside = false;
		}
		else if (origin[i] > max_b[i])
		{
			side[i] = 1;
			candidate_plane[i] = max_b[i];
			inside = false;
		}
		else
			side[i] = 0;
		//fprintf(stderr, "axis %i min_b: %.2f max_b: %.2f, side: %i\n", i, min_b[i], max_b[i], side[i]);
	}
	if (inside)
		return false;

	// Calculate the distance to the plane
	float max_t[3];
	for (i = 0; i < 3; i++)
	{
		if (side[i] != 0 && dir[i] != 0.)
			max_t[i] = (candidate_plane[i] - origin[i]) / dir[i];
		else
			max_t[i] = -1.0;
	}

	// Calculate the biggest distance
	int which_plane = 0;
	for (i = 0; i < 3; i++)
		if (max_t[which_plane] < max_t[i])
			which_plane = i;
	// In case we are facing away from the cube
	if (max_t[which_plane] < 0. || max_t[which_plane] > hit->distance)
		return false;
	// Check if our hit is on the cube or not
	for (i = 0; i < 3; i++)
	{
		if (which_plane != i)
		{
			float coord = origin[i] + dir[i] * max_t[which_plane];
			if ((coord < min_b[i]) || (coord > max_b[i]))
				return false;
		}
	}

	// Set values
	for (i = 0; i < 3; i++)
	{
		if (which_plane != i)
		{
			float coord = origin[i] + dir[i] * max_t[which_plane];
			((float*)&hit->location)[i] = coord;
			((float*)&hit->normal)[i] = 0;
		}
		else
		{
			((float*)&hit->location)[i] = candidate_plane[i];
			((float*)&hit->normal)[i] = side[i];
		}
	}
	hit->distance = max_t[which_plane];
	hit->color = data->color;

	quaternion_mult_vec3(&hit->normal, &object->transform.rotation, &hit->normal);

	quaternion_mult_vec3(&hit->location, &object->transform.rotation, &hit->location);
	vec3_add(&hit->location, &hit->location, &object->transform.position);

	return true;
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