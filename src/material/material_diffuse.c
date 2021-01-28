/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_diffuse.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 16:01:33 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include <math.h>

#define NORMAL_OFFSET 0.0001

static void add_diffuse_lighting(const t_scene* scene, t_vec3* position, t_vec3* normal, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	t_ray ray;
	vec3_scale(&ray.origin, normal, NORMAL_OFFSET);
	vec3_add(&ray.origin, position, &ray.origin);

	// Trace point lights
	for (size_t i = 0; i < scene->lights.count; i++)
	{
		t_light* light = list_index(&scene->lights, i);
		t_vec3 offset;
		t_vec3 normalized;

		vec3_subtract(&offset, &light->position, position);
		vec3_normalize(&normalized, &offset);
		float strength = vec3_dot(normal, &normalized);
		if (strength < 0)
			continue;

		float sqr_dist = vec3_magnitude_sqr(&offset);

		ray.direction = normalized;
		if (!cast_ray_max(scene, &ray, &hit, sqrtf(sqr_dist)))
		{
			strength *= 10 / sqr_dist;

			o_hdr->r += light->color.r * strength;
			o_hdr->g += light->color.g * strength;
			o_hdr->b += light->color.b * strength;
		}
	}

	// Trace directional lights
	for (size_t i = 0; i < scene->directional_lights.count; i++)
	{
		t_directional_light* light = list_index(&scene->directional_lights, i);
		float strength = vec3_dot(normal, &light->direction);
		if (strength < 0)
			continue;

		ray.direction = light->direction;
		if (!cast_ray(scene, &ray, &hit))
		{
			o_hdr->r += light->color.r * strength;
			o_hdr->g += light->color.g * strength;
			o_hdr->b += light->color.b * strength;
		}
	}
}

static void material_diffuse(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	const t_material_diffuse* diffuse = material_data;

	*o_hdr = scene->ambiant;
	add_diffuse_lighting(scene, &trace_data->hit->location, &trace_data->hit->normal, o_hdr);

	o_hdr->r = o_hdr->r * diffuse->color.r;
	o_hdr->g = o_hdr->g * diffuse->color.g;
	o_hdr->b = o_hdr->b * diffuse->color.b;
}

t_material* material_diffuse_new(const t_color_hdr* color)
{
	t_material* material = malloc(sizeof(t_material));
	if (material == NULL)
		return (NULL);
	t_material_diffuse* diffuse = malloc(sizeof(t_material_diffuse));
	if (diffuse == NULL)
	{
		free(material);
		return (NULL);
	}
	diffuse->color = *color;

	material->material_data = diffuse;
	material->material_func = material_diffuse;
	return (material);
}