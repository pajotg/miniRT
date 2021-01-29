/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_square.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:37 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 15:24:57 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "mini_rt_object_data.h"

/*
**	Pure memory
*/

bool ray_intersects_square(const t_object* object, const t_ray* ray, t_ray_hit* o_hit)
{
	t_object_square* data = object->object_data;
	t_vec3 normal;
	t_vec3 temp2;
	t_vec3 temp;

	quaternion_mult_vec3(&normal, &object->transform.rotation, vec3_forward());

	vec3_subtract(&temp, &ray->origin, &object->transform.position);
	float height = vec3_dot( &normal, &temp );
	float travel_distance = -height / vec3_dot( &normal, &ray->direction );

	if (travel_distance < 0 || travel_distance > o_hit->distance)
		return (false);

	// Temp contains the offset of the origin to the object
	vec3_scale(&temp2, &ray->direction, travel_distance);
	vec3_add(&temp, &temp, &temp2);

	// Temp now contains the hit position for a plane, but we need to ignore if it it outside the bounds
	t_vec3 side;
	quaternion_mult_vec3(&side, &object->transform.rotation, vec3_right());
	float dot = vec3_dot(&side, &temp);
	if (dot > data->extends || dot < -data->extends)
		return (false);

	// Thats one side done, do the other!
	// Its faster to use the cross product rather than do a secondairy quat * vec3 mult
	vec3_cross(&side, &side, &normal);
	dot = vec3_dot(&side, &temp);
	if (dot > data->extends || dot < -data->extends)
		return (false);

	// Alright! we hit!
	o_hit->distance = travel_distance;
	o_hit->object = (t_object*)object;
	if (height > 0)
		o_hit->normal = normal;
	else
	{
		o_hit->normal.x = -normal.x;
		o_hit->normal.y = -normal.y;
		o_hit->normal.z = -normal.z;
	}
	vec3_add(&o_hit->location, &temp, &object->transform.position);

	return (true);
}