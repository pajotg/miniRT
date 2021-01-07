/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   object_intersections.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:14:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/07 20:16:23 by jsimonis      ########   odam.nl         */
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
		vec3_init(&hit->normal, -hit->normal.x, -hit->normal.y, -hit->normal.z);
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
**	Pure memory
*/

bool ray_intersects_square(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_square* data = object->object_data;
	t_vec3 normal;
	t_vec3 temp2;
	t_vec3 temp;

	quaternion_mult_vec3(&normal, &object->transform.rotation, vec3_forward());

	vec3_subtract(&temp, &ray->origin, &object->transform.position);
	float height = vec3_dot( &normal, &temp );
	float travel_distance = -height / vec3_dot( &normal, &ray->direction );

	if (travel_distance < 0 || travel_distance > hit->distance)
		return (false);

	// Temp contains the offset of the origin to the object
	vec3_scale(&temp2, &ray->direction, travel_distance);
	vec3_add(&temp, &temp, &temp2);

	// Temp now contains the hit position for a plane, but we need to ignore if it it outside the bounds
	t_vec3 side;
	quaternion_mult_vec3(&side, &object->transform.rotation, vec3_right());
	float dot = vec3_dot(&side, &temp);
	if (dot > data->size || dot < -data->size)
		return (false);

	// Thats one side done, do the other!
	// Its faster to use the cross product rather than do a secondairy quat * vec3 mult
	vec3_cross(&side, &side, &normal);
	dot = vec3_dot(&side, &temp);
	if (dot > data->size || dot < -data->size)
		return (false);

	// Alright! we hit!
	hit->distance = travel_distance;
	hit->color = data->color;
	if (height > 0)
		hit->normal = normal;
	else
	{
		hit->normal.x = -normal.x;
		hit->normal.y = -normal.y;
		hit->normal.z = -normal.z;
	}
	vec3_add(&hit->location, &temp, &object->transform.position);

	return (true);
}

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
	vec3_init(&hit->normal, 0, 0, dir->z > 0 ? -1 : 1);

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
	if (out_z > data->height || out_z < -data->height)
	{
		if (ray_intersects_cylinder_top_and_bottom(data->height, data->radius, &origin, &dir, hit))
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

	vec3_init(&hit->normal,
		(origin.x + dir.x * dist) / data->radius,
		(origin.y + dir.y * dist) / data->radius,
		0
	);
	if (is_inside)
	{
		hit->normal.x = -hit->normal.x;
		hit->normal.y = -hit->normal.y;
	}
	quaternion_mult_vec3(&hit->normal, &object->transform.rotation, &hit->normal);
	return (true);
}

/*
typedef struct s_vec2
{
	float x;
	float y;
}			t_vec2;
static bool Sign(t_vec2 p1, t_vec2 p2, t_vec2 p3){
	return (p1.x - p3.x) * (p2.y - p3.y) < (p2.x - p3.x) * (p1.y - p3.y);
}
static bool IsPointInTri(t_vec2 pt, t_vec2 v1, t_vec2 v2, t_vec2 v3)
{
	// Must all have the same sign
	bool s = Sign(pt, v2, v3);
	return ((Sign(pt, v1, v2) == s) && (s == Sign(pt, v3, v1)));
}
*/
bool ray_intersects_triangle(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_triangle* data = object->object_data;

	/*
	t_vec3 normal;
	t_vec3 temp1;
	t_vec3 temp2;

	vec3_subtract(&temp1, &data->second_point, &object->transform.position);
	vec3_subtract(&temp1, &data->third_point, &object->transform.position);
	vec3_cross(&normal, &temp1, &temp2);
	*/
	(void)data;
	(void)ray;
	(void)hit;

	return false;
}

/*
** Found this code, read through it, understood it, looks really good, does backface culling
** https://web.archive.org/web/20090803054252/http://tog.acm.org/resources/GraphicsGems/gems/RayBox.c
** Also learned about the register keyword, seems usefull, will use it more often
** I am pretty sure this works in any dimension too, so that is cool, maybe make a 4D raytracer? :)))
** Welp, i thought a "square" meant a cube, later in the bonus there is a compound cube (6 squares), welp...
** I made a bonus thinking it was mandatory, oh wells
*/

bool ray_intersects_cube(t_object* object, t_ray* ray, t_ray_hit* hit)
{
	t_object_cube* data = object->object_data;
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
		return false;	// This probably aint good, TODO: Fix it (probably calculate from the 6 planes, not quats)

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