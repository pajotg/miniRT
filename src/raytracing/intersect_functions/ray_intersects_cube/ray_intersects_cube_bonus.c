/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_cube.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:12:38 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/08 20:47:02 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

// REEE THE NORM EVEN COMPLAINS ABOUT LONG COMMENTS
// Found this code, read through it, understood it, looks really good,
//does backface culling
// https://web.archive.org/web/20090803054252/http://tog.acm.org/resources/
//GraphicsGems/gems/RayBox.c
// Also learned about the register keyword, seems usefull
// I am pretty sure this works in any dimension too, so that is cool,
//maybe make a 4D raytracer? :)))
// Welp, i thought a "square" meant a cube,
//later in the bonus there is a compound cube (6 squares), welp...
// I made a bonus thinking it was mandatory, oh wells

// Set values
// Check if our hit is on the cube or not
// In case we are facing away from the cube
// for the outside we have the max time
// for the inside we have the min time
// Calculate the hit plane
// Calculate the distance to the plane
//fprintf(stderr, "axis %i min_b: %.2f max_b: %.2f, side: %i\n", i,
//min_b[sa->i],
//max_b[sa->i], side[sa->i]);
// Calculate which three planes we need to intersect

static bool	ric_validate_hit_point(t_cube_intersection_data *sa)
{
	sa->i = 0;
	while (sa->i < 3)
	{
		if (sa->which_plane != sa->i)
		{
			sa->coord = sa->origin[sa->i] + sa->dir[sa->i]
				 * sa->hit_t[sa->which_plane];
			if ((sa->coord < sa->min_b[sa->i]) || (sa->coord > sa->max_b[sa->i])
			)
				return (false);
		}
		sa->i++;
	}
	return (true);
}

static void	ric_calculate_hit_point_and_normal(t_cube_intersection_data *sa,
	t_ray_hit *o_hit)
{
	sa->i = 0;
	while (sa->i < 3)
	{
		if (sa->which_plane != sa->i)
		{
			sa->coord = sa->origin[sa->i] + sa->dir[sa->i]
				 * sa->hit_t[sa->which_plane];
			((float*)&o_hit->location)[sa->i] = sa->coord;
			((float*)&o_hit->normal)[sa->i] = 0;
		}
		else
		{
			((float*)&o_hit->location)[sa->i] = sa->candidate_plane[sa->i];
			((float*)&o_hit->normal)[sa->i] = sa->side[sa->i];
		}
		sa->i++;
	}
	if (sa->inside)
		vec3_negate(&o_hit->normal, &o_hit->normal);
}

bool	ray_intersects_cube(const t_object *object, const t_ray *ray,
	t_ray_hit *o_hit)
{
	t_cube_intersection_data	sa;

	ric_extract_data(&sa, object, ray);
	ric_calculate_sides(&sa);
	ric_fix_inside(&sa);
	ric_calculate_times(&sa);
	ric_calculate_hit_plane(&sa);
	if (sa.hit_t[sa.which_plane] < 0. || sa.hit_t[sa.which_plane] > o_hit
		->distance)
		return (false);
	if (!ric_validate_hit_point(&sa))
		return (false);
	ric_calculate_hit_point_and_normal(&sa, o_hit);
	o_hit->distance = sa.hit_t[sa.which_plane];
	o_hit->object = (t_object *)object;
	quaternion_mult_vec3(&o_hit->normal, &object->transform.rotation, &o_hit
		->normal);
	quaternion_mult_vec3(&o_hit->location, &object->transform.rotation, &o_hit
		->location);
	vec3_add(&o_hit->location, &o_hit->location, &object->transform.position);
	return (true);
}
