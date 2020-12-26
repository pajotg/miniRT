/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   object_intersections.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:14:12 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/26 14:41:23 by jasper        ########   odam.nl         */
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
	t_vec3 offset = vec3_subtract(object->transform.position, ray->origin);

	float dot = vec3_dot(ray->direction,offset);
	float delta = dot * dot - (vec3_magnitude_sqr(offset) - data->radius * data->radius);

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
	hit->location = vec3_add(ray->origin, vec3_scale(ray->direction, hit->distance));
	hit->normal = vec3_scale(vec3_subtract(hit->location, object->transform.position), 1 / data->radius);

	/*
	float sqrmag = vec3_magnitude_sqr(hit->normal);
	if (sqrmag > 1.01 || sqrmag < 0.99)
	{
		printf("Failed to calculate correct normal!\n");
		ft_printf("	Origin: %v\n", ray->origin);
		ft_printf("	Direction: %v\n", ray->direction);
		ft_printf("	Sphere pos: %v\n", object->transform.position);
		ft_printf("	Sphere radius: %v\n", data->radius);
		ft_printf("	Got offset: %v\n", offset);
		printf("	Got dot: %.2f\n", dot);
		printf("	Got delta: %.2f\n", delta);
		printf("	Got distance: %f\n", hit->distance);
		ft_printf("	Got location: %v\n", hit->location);
		ft_printf("	Got normal: %v\n", hit->normal);
		exit(1);
	}
	*/
	return true;
}

/*
**	Pure memory
*/

bool ray_intersects_plane(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_plane* data = object->object_data;

	t_vec3 normal = quaternion_mult_vec3(object->transform.rotation, vec3_new(0, 0, -1));
	float height = vec3_dot( normal, vec3_subtract(ray->origin, object->transform.position) );
	float travel_distance = -height / vec3_dot( normal, ray->direction );

	if (travel_distance < 0 || travel_distance > hit->distance)
		return false;

	hit->distance = travel_distance;
	hit->color = data->color;
	hit->location = vec3_add(ray->origin, vec3_scale(ray->direction, travel_distance));
	if (height > 0)
		hit->normal = normal;
	else
		hit->normal = vec3_subtract(vec3_new(0,0,0), normal);
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

	t_vec3 origin_vec = quaternion_mult_vec3(quaternion_conjugate(object->transform.rotation), vec3_subtract(ray->origin, object->transform.position));
	t_vec3 dir_vec = quaternion_mult_vec3(quaternion_conjugate(object->transform.rotation), ray->direction);
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