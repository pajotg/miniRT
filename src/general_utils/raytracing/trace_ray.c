/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_ray.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 10:44:42 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/25 11:35:08 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <math.h>

bool trace_ray(t_mlx_data* data, t_ray* ray, t_ray_hit* o_hit)
{
	bool has_hit = false;
	o_hit->distance = INFINITY;
	for (size_t i = 0; i < data->scene->objects.count; i++)
	{
		t_object* obj = darray_index(&data->scene->objects, i);
		if (obj->IntersectFunc(obj, ray, o_hit))
			has_hit = true;
	}
	return has_hit;
}

void trace_color(t_mlx_data* data, t_ray* ray, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	if (trace_ray(data, ray, &hit))
		*o_hdr = hit.color;
	else
		*o_hdr = data->scene->ambiant;
}

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
