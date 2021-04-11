/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_perfect_mirror.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:47:16 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
//#include "mini_rt_object.h"
#include <stdlib.h>
//#include <math.h>
#include "mini_rt_render_pixel.h"
//#include "libft.h"

#define NORMAL_OFFSET 0.001

// reflect

static void	material_perfect_mirror(const t_scene *scene, const void *
	material_data, const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	float							dot;
	t_ray							new_ray;
	t_color_hdr						incoming;
	t_vec3							temp;
	const t_material_perfect_mirror	*mirror = material_data;

	if (trace_data->depth > 5)
	{
		*o_hdr = scene->ambiant;
		return ;
	}
	new_ray.direction = trace_data->in_ray->direction;
	dot = vec3_dot(&new_ray.direction, &trace_data->hit->normal);
	vec3_scale(&temp, &trace_data->hit->normal, dot * -2);
	vec3_add(&new_ray.direction, &new_ray.direction, &temp);
	vec3_scale(&new_ray.origin, &trace_data->hit->normal, NORMAL_OFFSET);
	vec3_add(&new_ray.origin, &new_ray.origin, &trace_data->hit->location);
	trace_color(scene, &new_ray, trace_data->depth + 1, &incoming);
	o_hdr->r = mirror->color.r * incoming.r;
	o_hdr->g = mirror->color.g * incoming.g;
	o_hdr->b = mirror->color.b * incoming.b;
}

bool	material_perfect_mirror_init(t_material *material, const t_color_hdr *
	color)
{
	t_material_perfect_mirror	*perfect_mirror;

	perfect_mirror = malloc(sizeof(t_material_perfect_mirror));
	if (perfect_mirror == NULL)
		return (false);
	perfect_mirror->color = *color;
	*material = (t_material)
	{
		.material_data = perfect_mirror,
		.material_func = material_perfect_mirror,
		.material_free_data_func = free
	};
	return (true);
}
