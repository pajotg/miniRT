/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_ray.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 10:44:42 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 18:14:57 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_raycast.h"
#include "mini_rt_object.h"
#include <math.h>

// Things dont go well when we are EXACTLY on the bounds of a object, so just move the ray origin a bit away
#define NORMAL_OFFSET 0.01

static void trace_lights(t_scene* scene, t_vec3* position, t_vec3* normal, t_color_hdr* o_hdr)
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

void trace_color(t_scene* scene, t_ray* ray, t_color_hdr* o_hdr)
{
	t_ray_hit hit;
	t_color_hdr ambiant;
	if (cast_ray(scene, ray, &hit))
	{
		ambiant = scene->ambiant;
		trace_lights(scene, &hit.location, &hit.normal, &ambiant);

		o_hdr->r = hit.color.r * ambiant.r;
		o_hdr->g = hit.color.g * ambiant.g;
		o_hdr->b = hit.color.b * ambiant.b;
	}
	else
		*o_hdr = scene->ambiant;
}
