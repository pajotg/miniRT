/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_aabb.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/07 17:18:14 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 14:57:54 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "mini_rt_object_data.h"

bool ray_intersects_aabb(const t_ray* ray, const t_aabb* aabb, float max_dist)
{
	register int i;
	char side[3];
	float candidate_plane[3];
	bool inside = true;

	float* min_b = (float*)&aabb->min;
	float* max_b = (float*)&aabb->max;

	float* origin = (float*)&ray->origin;
	float* dir = (float*)&ray->direction;

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
	}
	if (inside)
		return true;

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
	if (max_t[which_plane] < 0. || max_t[which_plane] > max_dist)
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
	return true;
}