/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_triangle.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:04 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 15:24:58 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "mini_rt_object_data.h"

/*
**typedef struct s_vec2
**{
**	float x;
**	float y;
**}			t_vec2;
**static bool Sign(t_vec2 p1, t_vec2 p2, t_vec2 p3){
**	return (p1.x - p3.x) * (p2.y - p3.y) < (p2.x - p3.x) * (p1.y - p3.y);
**}
**static bool IsPointInTri(t_vec2 pt, t_vec2 v1, t_vec2 v2, t_vec2 v3)
**{
**	// Must all have the same sign
**	bool s = Sign(pt, v2, v3);
**	return ((Sign(pt, v1, v2) == s) && (s == Sign(pt, v3, v1)));
**}
*/

/*
**	Möller–Trumbore intersection
*/

bool ray_intersects_triangle(t_object* object, t_ray* ray, t_ray_hit* o_hit)
{
	const float EPSILON = 0.0000001;

	t_object_triangle* data = object->object_data;

	t_vec3 edge1;
	t_vec3 edge2;
	t_vec3 h;

	vec3_subtract(&edge1, &data->second_point, &object->transform.position);
	vec3_subtract(&edge2, &data->third_point, &object->transform.position);
	vec3_cross(&h, &ray->direction, &edge2);

	// parallel
	float a = vec3_dot(&edge1, &h);
	if (a > -EPSILON && a < EPSILON)
		return (false);

	float f = 1/a;
	t_vec3 s;
	vec3_subtract(&s, &ray->origin, &object->transform.position);
	float u = f * vec3_dot(&s, &h);
	if (u < 0 || u > 1)
		return (false);

	t_vec3 q;
	vec3_cross(&q, &s, &edge1);
	float v = f * vec3_dot(&ray->direction, &q);
	if (v < 0 || u + v > 1)
		return (false);

	t_vec3 normal;
	vec3_cross(&normal, &edge1, &edge2);
	vec3_normalize(&normal, &normal);

	float distance = f * vec3_dot(&edge2, &q);
	if (distance < 0 || distance > o_hit->distance)
		return (false);

	o_hit->distance = distance;
	o_hit->object = (t_object*)object;
	if (vec3_dot(&normal, &ray->direction) < 0)
		o_hit->normal = normal;
	else
		o_hit->normal = (t_vec3) { -normal.x, -normal.y, -normal.z };
	vec3_scale(&o_hit->location, &ray->direction, distance);
	vec3_add(&o_hit->location, &o_hit->location, &ray->origin);

	return (true);
}
