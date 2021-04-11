/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_intersects_cylinder.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 21:13:25 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 22:14:39 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"
//#include "ft_ternary.h"

/*
**	Woah, that took FOREVER, so manny mistakes
**	https://math.stackexchange.com/questions/2613781/line-cylinder-intersection
*/

// 1 = we hit something, return true
// 0 = we are going to hit NOTHING!
// -1 = we did not hit the top or bottom, but maybe still hit the inside
static int	ric_intersects_only_top_or_bottom(t_cylinder_intersection_data *cid,
	const t_object *
	object, t_ray_hit *o_hit)
{
	if (ric_ray_intersects_cylinder_top_and_bottom((t_cylinder_caps_args)
			{cid->data->height_extends, cid->data ->radius, &cid->origin,
				&cid->dir, o_hit}))
	{
		o_hit->object = (t_object *)object;
		quaternion_mult_vec3(&o_hit->location, &object->transform.rotation,
			&o_hit->location);
		quaternion_mult_vec3(&o_hit->normal, &object->transform.rotation,
			&o_hit->normal);
		vec3_add(&o_hit->location, &o_hit->location, &object
			->transform.position);
		return (1);
	}
	if (cid->origin.z > cid->data->height_extends || cid->origin.z < -cid
		->data
		->height_extends)
		return (0);
	return (-1);
}

static bool	ric_magic_distance_math(t_cylinder_intersection_data *cid,
	t_ray_hit *o_hit)
{
	cid->dir2d_sqr = cid->dir.x * cid->dir.x + cid->dir.y * cid->dir.y;
	if (cid->dir2d_sqr <= 0)
		return (false);
	cid->t = - (cid->origin.x * cid->dir.x + cid->dir.y * cid->origin.y)
		 /cid->dir2d_sqr;
	cid->dsqr = cid->origin.x * cid->origin.x + cid->origin.y * cid->origin.y
		 -cid->t * cid->t
		 *cid->dir2d_sqr;
	if (cid->dsqr > cid->data->radius * cid->data->radius)
		return (false);
	cid->k = sqrtf((cid->data->radius * cid->data->radius - cid->dsqr)
			 /cid->dir2d_sqr);
	cid->is_inside = cid->t < cid->k;
	if (cid->is_inside)
		cid->k = -cid->k;
	cid->dist = cid->t - cid->k;
	if (cid->dist > o_hit->distance || cid->dist < 0)
		return (false);
	cid->out_z = cid->origin.z + cid->dir.z * cid->dist;
	return (true);
}

static bool	ric_add_top_and_bottom(t_cylinder_intersection_data *cid,
	const t_object *
	object, t_ray_hit *o_hit)
{
	if (ric_ray_intersects_cylinder_top_and_bottom((t_cylinder_caps_args)
			{cid->data->height_extends,
				cid->data ->radius, &cid->origin, &cid->dir, o_hit}))
	{
		o_hit->object = (t_object *)object;
		quaternion_mult_vec3(&o_hit->location, &object->transform.rotation,
			&o_hit->location);
		quaternion_mult_vec3(&o_hit->normal, &object->transform.rotation,
			&o_hit->normal);
		vec3_add(&o_hit->location, &o_hit->location, &object
			->transform.position);
		return (true);
	}
	return (false);
}

static void	ric_calculate_hit_result(t_cylinder_intersection_data *cid,
	const t_object *
	object, const t_ray *ray, t_ray_hit *o_hit)
{
	o_hit->distance = cid->dist;
	o_hit->object = (t_object *)object;
	vec3_scale(&o_hit->location, &ray->direction, cid->dist);
	vec3_add(&o_hit->location, &o_hit->location, &ray->origin);
	o_hit->normal = (t_vec3){
		(cid->origin.x + cid->dir.x * cid->dist) / cid->data->radius,
		(cid->origin.y + cid->dir.y * cid->dist) / cid->data->radius,
		0
	};
	if (cid->is_inside)
	{
		o_hit->normal.x = -o_hit->normal.x;
		o_hit->normal.y = -o_hit->normal.y;
	}
	quaternion_mult_vec3(&o_hit->normal, &object->transform.rotation, &o_hit
		->normal);
}
// If we are above or below the height, raycast to the top and bottom
// Yeah, i should watch out for this kind of stuff more often
// We can only return false if we are not inside the cylinder
// If we are above or below, first check top/bottom

bool	ray_intersects_cylinder(const t_object *object, const t_ray *ray,
	t_ray_hit *o_hit)
{
	int								code;
	t_cylinder_intersection_data	cid;

	ric_calculate_local_space(&cid, object, ray);
	if (cid.origin.x * cid.origin.x + cid.origin.y * cid.origin.y < cid.data
		->radius * cid.data->radius)
	{
		code = ric_intersects_only_top_or_bottom(&cid, object, o_hit);
		if (code == 1)
			return (true);
		else if (code == 0)
			return (false);
	}
	if (!ric_magic_distance_math(&cid, o_hit))
		return (false);
	if (cid.out_z > cid.data->height_extends || cid.out_z < -cid.data
		->height_extends)
		return (ric_add_top_and_bottom(&cid, object, o_hit));
	ric_calculate_hit_result(&cid, object, ray, o_hit);
	return (true);
}
