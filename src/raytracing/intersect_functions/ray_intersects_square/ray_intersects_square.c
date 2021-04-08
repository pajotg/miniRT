/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_square.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:37 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/08 22:46:05 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

/*
**	Pure memory
*/

// Alright! we hit!
// Its faster to use the cross product rather than do a secondairy quat * vec3
//mult
// Thats one side done, do the other!
// Temp now contains the hit position for a plane, but we need to ignore if it
//it outside the bounds
// Temp contains the offset of the origin to the object

static bool	calculate_hit_point(t_square_intersection_data *sid,
	const t_object *object, const t_ray *ray, t_ray_hit *o_hit)
{
	quaternion_mult_vec3(&sid->normal, &object->transform.rotation,
		vec3_forward(
			));
	vec3_subtract(&sid->temp, &ray->origin, &object->transform.position);
	sid->height = vec3_dot(&sid->normal, &sid->temp);
	sid->travel_distance = -sid->height / vec3_dot(&sid->normal, &ray
			->direction);
	if (sid->travel_distance < 0 || sid->travel_distance > o_hit->distance)
		return (false);
	vec3_scale(&sid->temp2, &ray->direction, sid->travel_distance);
	vec3_add(&sid->temp, &sid->temp, &sid->temp2);
	quaternion_mult_vec3(&sid->side, &object->transform.rotation, vec3_right());
	return (true);
}

bool	ray_intersects_square(const t_object *object, const t_ray *ray,
	t_ray_hit *o_hit)
{
	t_square_intersection_data	sid;

	sid.data = object->object_data;
	if (!calculate_hit_point(&sid, object, ray, o_hit))
		return (false);
	sid.dot = vec3_dot(&sid.side, &sid.temp);
	if (sid.dot > sid.data->extends || sid.dot < -sid.data->extends)
		return (false);
	vec3_cross(&sid.side, &sid.side, &sid.normal);
	sid.dot = vec3_dot(&sid.side, &sid.temp);
	if (sid.dot > sid.data->extends || sid.dot < -sid.data->extends)
		return (false);
	o_hit->distance = sid.travel_distance;
	o_hit->object = (t_object *)object;
	if (sid.height > 0)
		o_hit->normal = sid.normal;
	else
	{
		o_hit->normal.x = -sid.normal.x;
		o_hit->normal.y = -sid.normal.y;
		o_hit->normal.z = -sid.normal.z;
	}
	vec3_add(&o_hit->location, &sid.temp, &object->transform.position);
	return (true);
}
