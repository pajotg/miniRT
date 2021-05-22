/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_triangle.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:04 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/08 22:18:01 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

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

// parallel

static bool	calculate_datas(t_triangle_intersection_data *tid, t_object *object,
	t_ray *ray, t_ray_hit *o_hit)
{
	vec3_subtract(&tid->edge1, &tid->data->second_point, &object
		->transform.position);
	vec3_subtract(&tid->edge2, &tid->data->third_point, &object
		->transform.position);
	vec3_cross(&tid->h, &ray->direction, &tid->edge2);
	tid->a = vec3_dot(&tid->edge1, &tid->h);
	if (tid->a > -EPSILON && tid->a < EPSILON)
		return (false);
	tid->f = 1 / tid->a;
	vec3_subtract(&tid->s, &ray->origin, &object->transform.position);
	tid->u = tid->f * vec3_dot(&tid->s, &tid->h);
	if (tid->u < 0 || tid->u > 1)
		return (false);
	vec3_cross(&tid->q, &tid->s, &tid->edge1);
	tid->v = tid->f * vec3_dot(&ray->direction, &tid->q);
	if (tid->v < 0 || tid->u + tid->v > 1)
		return (false);
	vec3_cross(&tid->normal, &tid->edge1, &tid->edge2);
	vec3_normalize(&tid->normal, &tid->normal);
	tid->distance = tid->f * vec3_dot(&tid->edge2, &tid->q);
	if (tid->distance < 0 || tid->distance > o_hit->distance)
		return (false);
	return (true);
}

bool	ray_intersects_triangle(t_object *object, t_ray *ray, t_ray_hit *o_hit)
{
	t_triangle_intersection_data	tid;

	tid.data = object->object_data;
	if (!calculate_datas(&tid, object, ray, o_hit))
		return (false);
	o_hit->distance = tid.distance;
	o_hit->object = (t_object *)object;
	if (vec3_dot(&tid.normal, &ray->direction) < 0)
		o_hit->normal = tid.normal;
	else
		o_hit->normal = (t_vec3){-tid.normal.x, -tid.normal.y, -tid.normal.z};
	vec3_scale(&o_hit->location, &ray->direction, tid.distance);
	vec3_add(&o_hit->location, &o_hit->location, &ray->origin);
	return (true);
}
