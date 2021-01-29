/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_color.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 10:44:42 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/29 13:23:14 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_raycast.h"
#include "mini_rt_object.h"
#include <math.h>

void trace_color(t_scene* scene, t_ray* ray, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	t_material_trace_data trace_data;
	if (cast_ray(scene, ray, &hit))
	{
		trace_data = (t_material_trace_data)
		{
			.depth = 0,
			.hit = &hit,
			.in_ray = ray
		};
		t_material *material = hit.object->material->ptr;
		material->material_func(
			scene,
			material->material_data,
			&trace_data,
			o_hdr
		);
	}
	else
		*o_hdr = scene->ambiant;
}
