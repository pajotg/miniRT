/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_ray.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 10:44:42 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/25 14:23:35 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <math.h>

static bool trace_ray_raw(t_mlx_data* data, t_ray* ray, t_ray_hit* o_hit)
{
	bool has_hit = false;
	for (size_t i = 0; i < data->scene->objects.count; i++)
	{
		t_object* obj = darray_index(&data->scene->objects, i);
		if (obj->IntersectFunc(obj, ray, o_hit))
			has_hit = true;
	}
	return has_hit;
}

bool trace_ray(t_mlx_data* data, t_ray* ray, t_ray_hit* o_hit)
{
	o_hit->distance = INFINITY;
	return trace_ray_raw(data, ray, o_hit);
}

bool trace_ray_max(t_mlx_data* data, t_ray* ray, t_ray_hit* o_hit, float max_dist)
{
	o_hit->distance = max_dist;
	return trace_ray_raw(data, ray, o_hit);
}

#include <stdio.h>
#include <stdlib.h>
void trace_ambiant(t_mlx_data* data, t_vec3* position, t_vec3* normal, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	t_ray ray;
	ray.origin = vec3_add(*position, vec3_scale(*normal, 0.01));

	*o_hdr = data->scene->ambiant;
	for (size_t i = 0; i < data->scene->lights.count; i++)
	{
		t_light* light = darray_index(&data->scene->lights, i);
		t_vec3 offset = vec3_subtract(light->position, *position);
		t_vec3 normalized = vec3_normalize(offset);
		float strength = vec3_dot(*normal, normalized);
		if (strength < 0)
			continue;

		float sqr_dist = vec3_magnitude_sqr(offset);

		ray.direction = normalized;
		if (!trace_ray_max(data, &ray, &hit, sqrtf(sqr_dist)) || true)
		{
			strength *= 10 / sqr_dist;

			o_hdr->r += light->color.r * strength;
			o_hdr->g += light->color.g * strength;
			o_hdr->b += light->color.b * strength;
		}

	}
	(void)normal;
}

void trace_color(t_mlx_data* data, t_ray* ray, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	if (trace_ray(data, ray, &hit))
	{
		//*
		t_color_hdr ambiant;
		trace_ambiant(data, &hit.location, &hit.normal, &ambiant);

		o_hdr->r = hit.color.r * ambiant.r;
		o_hdr->g = hit.color.g * ambiant.g;
		o_hdr->b = hit.color.b * ambiant.b;
		//*/

		/*
		o_hdr->r = hit.normal.x;
		o_hdr->g = hit.normal.y;
		o_hdr->b = hit.normal.z;
		//*/
	}
	else
		*o_hdr = data->scene->ambiant;
}

/*
** Dont need to search up how, i can just use ALGEBRA!
** asin(0.5 / dist) = cam.fov / 2	// angle of ray = desired angle
** 0.5 / dist = sin(cam.fov / 2)
** 0.5 = sin(cam.fov / 2) * dist
** 0.5 / sin(cam.fov / 2) = dist
*/

void pix_to_ray(t_mlx_data* data, int x, int y, t_ray* o_ray)
{
	float fov_axis = data->scene->resolution.width;
	float ox = (data->scene->resolution.width / 2.0 - x) / fov_axis;
	float oy = (data->scene->resolution.height / 2.0 - y) / fov_axis;

	// ox and oy are in the range -0.5 to 0.5

	t_camera* cam = darray_index(&data->scene->cameras, data->scene->current_camera_index);
	float dist = 0.5 / sin(cam->fov / 2);

	t_vec3 dir = vec3_normalize(vec3_add(
		vec3_add(
			vec3_scale(quaternion_mult_vec3(cam->transform.rotation, vec3_new(1,0,0)), ox),
			vec3_scale(quaternion_mult_vec3(cam->transform.rotation, vec3_new(0,1,0)), oy)
		),
		vec3_scale(quaternion_mult_vec3(cam->transform.rotation, vec3_new(0,0,-1)), dist)
	));

	o_ray->origin = cam->transform.position;
	o_ray->direction = dir;
}
