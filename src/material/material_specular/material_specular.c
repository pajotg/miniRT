/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_specular.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/08 18:22:17 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

static float	get_strength(const t_vec3 *reflected_dir, const t_vec3 *
	light_dir, float pow)
{
	float	dot;

	dot = vec3_dot(reflected_dir, light_dir);
	if (dot < 0)
		return (0);
	dot = powf(dot, pow);
	return (dot);
}

// That one if statement,, UHHHHHHHH

static void	add_point_lighting(t_add_lighting_data data)
{
	t_struct_abse	sa;

	sa.i = 0;
	while (sa.i < data.scene->lights.count)
	{
		sa.light = list_index(&data.scene->lights, sa.i);
		vec3_subtract(&sa.offset, &sa.light->position, &data.ray->origin);
		vec3_normalize(&sa.normalized, &sa.offset);
		sa.strength = get_strength(data.reflected_dir, &sa.normalized,
				data.mat->pow);
		if (sa.strength > 0)
		{
			sa.sqr_dist = vec3_magnitude_sqr(&sa.offset);
			data.ray->direction = sa.normalized;
			if (!cast_ray_max(data.scene, data.ray, &sa.hit, sqrtf(sa.sqr_dist))
			)
			{
				sa.strength *= 10 / sa.sqr_dist;
				data.o_hdr->r += sa.light->color.r * sa.strength;
				data.o_hdr->g += sa.light->color.g * sa.strength;
				data.o_hdr->b += sa.light->color.b * sa.strength;
			}
		}
		sa.i++;
	}
}

static void	add_directional_lighting(t_add_lighting_data data)
{
	size_t				i;
	float				strength;
	t_directional_light	*light;
	t_ray_hit			hit;

	i = 0;
	while (i < data.scene->directional_lights.count)
	{
		light = list_index(&data.scene->directional_lights, i);
		strength = get_strength(data.reflected_dir, &light->direction,
				data.mat
				->pow);
		if (strength > 0)
		{
			data.ray->direction = light->direction;
			if (!cast_ray(data.scene, data.ray, &hit))
			{
				data.o_hdr->r += light->color.r * strength;
				data.o_hdr->g += light->color.g * strength;
				data.o_hdr->b += light->color.b * strength;
			}
		}
		i++;
	}
}

// Trace directional lights
// Trace point lights

static void	add_specular_lighting(t_lighting_data data)
{
	t_ray	ray;

	vec3_scale(&ray.origin, data.reflected_dir, NORMAL_OFFSET);
	vec3_add(&ray.origin, data.position, &ray.origin);
	add_point_lighting((t_add_lighting_data){data.scene, &ray,
		data.reflected_dir, data.mat, data.o_hdr});
	add_directional_lighting((t_add_lighting_data){data.scene, &ray,
		data.reflected_dir, data.mat, data.o_hdr});
}

void	material_specular(const t_scene *scene, const void *material_data,
	const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	float						dot;
	const t_material_specular	*specular = material_data;
	t_vec3						reflected_dir;

	dot = vec3_dot(&trace_data->in_ray->direction, &trace_data->hit
			->normal);
	vec3_scale(&reflected_dir, &trace_data->hit->normal, dot * -2);
	vec3_add(&reflected_dir, &trace_data->in_ray->direction, &reflected_dir);
	*o_hdr = (t_color_hdr){0, 0, 0 };
	add_specular_lighting((t_lighting_data){scene, &trace_data->hit->location,
		&reflected_dir,
		specular, o_hdr});
	o_hdr->r *= specular->color.r;
	o_hdr->g *= specular->color.g;
	o_hdr->b *= specular->color.b;
}
