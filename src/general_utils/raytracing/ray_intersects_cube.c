/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_cube.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:12:38 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/17 13:46:57 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_objects.h"
#include "mini_rt_color_math_utils.h"
#include <stdbool.h>
#include <math.h>

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
		min_b[i] = - data->extends;
		max_b[i] = + data->extends;
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