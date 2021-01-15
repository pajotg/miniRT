/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_plane.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/15 21:14:26 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_objects.h"
#include "mini_rt_color_math_utils.h"
#include <stdbool.h>
#include <math.h>

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
