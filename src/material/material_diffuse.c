/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_diffuse.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/05 12:53:50 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_render_pixel.h"
#include "libft.h"

#define NORMAL_OFFSET 0.001

#define NUM_SAMPLES_PER_RAY 1
#define MAX_BOUNCES 4

static t_vec3 random_on_sphere()
{
	float theta = ft_randf() * 2 * M_PI;
	float phi = acosf(1 - 2 * ft_randf());
	return (t_vec3)
	{
		.x = sin(phi) * cos(theta),
		.y = cos(phi),
		.z = sin(phi) * sin(theta),
	};
}

static t_vec3 random_in_hemisphere(const t_vec3* normal)
{
	t_vec3 dir = random_on_sphere();
	float dot = vec3_dot(&dir, normal);
	if (dot < 0)
	{
		t_vec3 temp;
		vec3_scale(&temp, normal, dot * -2);
		vec3_add(&dir, &dir, &temp);
	}
	return (dir);
}

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

static void material_diffuse_gi(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	const t_material_diffuse* diffuse = material_data;
	if (trace_data->depth > 5)
	{
		*o_hdr = scene->ambiant;
		return ;
	}

	*o_hdr = scene->ambiant;
	add_diffuse_lighting(scene, &trace_data->hit->location, &trace_data->hit->normal, o_hdr);

	t_ray new_ray;
	t_color_hdr incoming;

	new_ray.direction = random_in_hemisphere(&trace_data->hit->normal);
	vec3_scale(&new_ray.origin, &trace_data->hit->normal, NORMAL_OFFSET);
	vec3_add(&new_ray.origin, &new_ray.origin, &trace_data->hit->location);

	const float p = 1.0 / (2.0 * M_PI);	// probability of new_ray

	// Compute the BRDF for this ray
	float cos_theta = vec3_dot(&new_ray.direction, &trace_data->hit->normal);
	t_color_hdr BRDF = (t_color_hdr) {
		.r = diffuse->color.r / M_PI,
		.g = diffuse->color.g / M_PI,
		.b = diffuse->color.b / M_PI
	};
	trace_color(scene, &new_ray, trace_data->depth + 1, &incoming);

	o_hdr->r += BRDF.r * incoming.r * cos_theta / p;
	o_hdr->g += BRDF.g * incoming.g * cos_theta / p;
	o_hdr->b += BRDF.b * incoming.b * cos_theta / p;
}

static void material_diffuse(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	const t_material_diffuse* diffuse = material_data;

	*o_hdr = scene->ambiant;
	add_diffuse_lighting(scene, &trace_data->hit->location, &trace_data->hit->normal, o_hdr);
	o_hdr->r *= diffuse->color.r;
	o_hdr->g *= diffuse->color.g;
	o_hdr->b *= diffuse->color.b;
}

static void material_diffuse_switch(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	if (scene->use_gi)
		material_diffuse_gi(scene, material_data, trace_data, o_hdr);
	else
		material_diffuse(scene, material_data, trace_data, o_hdr);
}

bool material_diffuse_init(t_material* material, const t_color_hdr* color)
{
	t_material_diffuse* diffuse = malloc(sizeof(t_material_diffuse));
	if (diffuse == NULL)
		return (false);
	diffuse->color = *color;

	*material = (t_material)
	{
		.material_data = diffuse,
		.material_func = material_diffuse_switch,
		.material_free_data_func = free
	};
	return (true);
}