/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_aabb.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/07 17:18:14 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/29 16:52:02 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "mini_rt_object_data.h"

// In my opinion the function looked way better before the normification

// Calculate which three planes we need to intersect
static bool	calculate_sides(char *side, float *candidate_plane, float *origin,
	const t_aabb *aabb)
{
	register int	i;
	bool			inside;

	inside = true;
	i = 0;
	while (i < 3)
	{
		if (origin[i] < ((float *)&aabb->min)[i])
		{
			side[i] = -1;
			candidate_plane[i] = ((float *)&aabb->min)[i];
			inside = false;
		}
		else if (origin[i] > ((float *)&aabb->max)[i])
		{
			side[i] = 1;
			candidate_plane[i] = ((float *)&aabb->max)[i];
			inside = false;
		}
		else
			side[i] = 0;
		i++;
	}
	return (inside);
}

// Calculate the distance to the plane

static bool	calculate_distances(const t_ray *ray, const t_aabb *aabb, float *
	max_t)
{
	float			*origin;
	float			*dir;
	char			side[3];
	float			candidate_plane[3];
	register int	i;

	origin = (float *)&ray->origin;
	dir = (float *)&ray->direction;
	if (calculate_sides(side, candidate_plane, origin, aabb))
		return (true);
	i = 0;
	while (i < 3)
	{
		if (side[i] != 0 && dir[i] != 0.)
			max_t[i] = (candidate_plane[i] - origin[i]) / dir[i];
		else
			max_t[i] = -1.0;
		i++;
	}
	return (false);
}

static int	calculate_which_plane(float *max_t)
{
	register int	i;
	int				which_plane;

	i = 0;
	which_plane = 0;
	while (i < 3)
	{
		if (max_t[which_plane] < max_t[i])
			which_plane = i;
		i++;
	}
	return (which_plane);
}

// Check if our hit is on the cube or not
// In case we are facing away from the cube
// Calculate the biggest distance

bool	ray_intersects_aabb(const t_ray *ray, const t_aabb *aabb,
	float max_dist)
{
	float			coord;
	float			max_t[3];
	int				which_plane;
	register int	i;

	if (calculate_distances(ray, aabb, max_t))
		return (true);
	which_plane = calculate_which_plane(max_t);
	if (max_t[which_plane] < 0. || max_t[which_plane] > max_dist)
		return (false);
	i = 0;
	while (i < 3)
	{
		if (which_plane != i)
		{
			coord = ((float *)&ray->origin)[i] + ((float *)&ray->direction)[i]
				 * max_t[which_plane];
			if ((coord < ((float *)&aabb->min)[i]) || (coord > ((float *)&aabb
						->max)[i]))
				return (false);
		}
		i++;
	}
	return (true);
}
