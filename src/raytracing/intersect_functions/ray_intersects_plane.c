/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_plane.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:33:22 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
//#include "mini_rt_object_data.h"

/*
**	Pure memory
*/

//t_object_plane* data = object->object_data;

bool	ray_intersects_plane(const t_object *object, const t_ray *ray,
	t_ray_hit *o_hit)
{
	float	height;
	float	travel_distance;
	t_vec3	normal;
	t_vec3	temp;

	quaternion_mult_vec3(&normal, &object->transform.rotation, vec3_forward());
	vec3_subtract(&temp, &ray->origin, &object->transform.position);
	height = vec3_dot(&normal, &temp);
	travel_distance = -height / vec3_dot(&normal, &ray->direction);
	if (travel_distance < 0 || travel_distance > o_hit->distance)
		return (false);
	o_hit->distance = travel_distance;
	o_hit->object = (t_object *)object;
	vec3_scale(&temp, &ray->direction, travel_distance);
	vec3_add(&o_hit->location, &ray->origin, &temp);
	if (height > 0)
		o_hit->normal = normal;
	else
	{
		o_hit->normal.x = -normal.x;
		o_hit->normal.y = -normal.y;
		o_hit->normal.z = -normal.z;
	}
	return (true);
}
