/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_color.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 10:44:42 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/28 16:00:31 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_raycast.h"
#include "mini_rt_object.h"
#include <math.h>

void trace_color(t_scene* scene, t_ray* ray, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	//t_color_hdr ambiant;
	if (cast_ray(scene, ray, &hit))
	{
		//ambiant = scene->ambiant;
		//trace_lights(scene, &hit.location, &hit.normal, &ambiant);

		t_material_trace_data trace_data = (t_material_trace_data)
		{
			.depth = 0,
			.hit = &hit,
			.in_ray = ray
		};
		hit.object->material->material_func(
			scene,
			hit.object->material->material_data,
			&trace_data,
			o_hdr
		);

		/*
		o_hdr->r = hit.color.r * ambiant.r;
		o_hdr->g = hit.color.g * ambiant.g;
		o_hdr->b = hit.color.b * ambiant.b;
		*/
	}
	else
		*o_hdr = scene->ambiant;
}
