/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_diffuse.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 14:42:14 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

#define NUM_SAMPLES_PER_RAY 1
#define MAX_BOUNCES 4

// Compute the BRDF for this ray
// probability of new_ray

const float	g_p = 1.0 / (2.0 * M_PI);

static void	material_diffuse_gi(const t_scene *scene, const void *material_data,
	const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	float						cos_theta;
	t_ray						new_ray;
	t_color_hdr					incoming;
	t_color_hdr					BRDF;
	const t_material_diffuse	*diffuse = material_data;

	*o_hdr = scene->ambiant;
	if (trace_data->depth > 5)
		return ;
	add_diffuse_lighting(scene, &trace_data->hit->location, &trace_data->hit
		->normal, o_hdr);
	new_ray.direction = random_in_hemisphere(&trace_data->hit->normal);
	vec3_scale(&new_ray.origin, &trace_data->hit->normal, NORMAL_OFFSET);
	vec3_add(&new_ray.origin, &new_ray.origin, &trace_data->hit->location);
	cos_theta = vec3_dot(&new_ray.direction, &trace_data->hit->normal);
	BRDF = (t_color_hdr){
		.r = diffuse->color.r / M_PI,
		.g = diffuse->color.g / M_PI,
		.b = diffuse->color.b / M_PI
	};
	trace_color(scene, &new_ray, trace_data->depth + 1, &incoming);
	o_hdr->r += BRDF.r * incoming.r * cos_theta / g_p;
	o_hdr->g += BRDF.g * incoming.g * cos_theta / g_p;
	o_hdr->b += BRDF.b * incoming.b * cos_theta / g_p;
}

static void	material_diffuse(const t_scene *scene, const void *material_data,
	const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	const t_material_diffuse	*diffuse = material_data;

	*o_hdr = scene->ambiant;
	add_diffuse_lighting(scene, &trace_data->hit->location, &trace_data->hit
		->normal, o_hdr);
	o_hdr->r *= diffuse->color.r;
	o_hdr->g *= diffuse->color.g;
	o_hdr->b *= diffuse->color.b;
}

static void	material_diffuse_switch(const t_scene *scene, const void *
	material_data, const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	if (scene->use_gi)
		material_diffuse_gi(scene, material_data, trace_data, o_hdr);
	else
		material_diffuse(scene, material_data, trace_data, o_hdr);
}

bool	material_diffuse_init(t_material *material, const t_color_hdr *color)
{
	t_material_diffuse	*diffuse;

	diffuse = malloc(sizeof(t_material_diffuse));
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
