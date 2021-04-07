/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_specular.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/07 13:07:32 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt_material_data.h"
# include "mini_rt_object.h"
# include <stdlib.h>
# include <math.h>
# include "mini_rt_render_pixel.h"
# include "libft.h"

# define NORMAL_OFFSET 0.001

static float get_strength(const t_vec3* reflected_dir, const t_vec3* light_dir, float pow)
{
	float dot = vec3_dot(reflected_dir, light_dir);
	if (dot < 0)
		return 0;
	dot = powf(dot, pow);
	return dot;
}

static void	add_point_lighting(const t_scene *scene, t_ray *ray,
	const t_vec3 *reflected_dir, const t_material_specular* mat, t_color_hdr *o_hdr)
{
	size_t i = 0;
	t_light* light;
	t_vec3 offset;
	t_vec3 normalized;
	float strength;
	float sqr_dist;
	t_ray_hit hit;

	while (i < scene->lights.count)
	{
		light = list_index(&scene->lights, i);
		vec3_subtract(&offset, &light->position, &ray->origin);
		vec3_normalize(&normalized, &offset);
		strength = get_strength(reflected_dir, &normalized, mat->pow);
		if (strength > 0)
		{
			sqr_dist = vec3_magnitude_sqr(&offset);
			ray->direction = normalized;
			if (!cast_ray_max(scene, ray, &hit, sqrtf(sqr_dist)))
			{
				strength *= 10 / sqr_dist;
				o_hdr->r += light->color.r * strength;
				o_hdr->g += light->color.g * strength;
				o_hdr->b += light->color.b * strength;
			}
		}
		i++;
	}
}

static void	add_directional_lighting(const t_scene *scene, t_ray *ray,
	const t_vec3 *reflected_dir, const t_material_specular* mat, t_color_hdr *o_hdr)
{
	size_t				i;
	float				strength;
	t_directional_light	*light;
	t_ray_hit			hit;

	i = 0;
	while (i < scene->directional_lights.count)
	{
		light = list_index(&scene->directional_lights, i);
		strength = get_strength(reflected_dir, &light->direction, mat->pow);
		if (strength > 0)
		{
			ray->direction = light->direction;
			if (!cast_ray(scene, ray, &hit))
			{
				o_hdr->r += light->color.r * strength;
				o_hdr->g += light->color.g * strength;
				o_hdr->b += light->color.b * strength;
			}
		}
		i++;
	}
}

// Trace directional lights
// Trace point lights

void	add_specular_lighting(const t_scene *scene, t_vec3 *position,
	const t_vec3 *reflected_dir, const t_material_specular* mat, t_color_hdr *o_hdr)
{
	t_ray	ray;

	vec3_scale(&ray.origin, reflected_dir, NORMAL_OFFSET);
	vec3_add(&ray.origin, position, &ray.origin);

	add_point_lighting(scene, &ray, reflected_dir, mat, o_hdr);
	add_directional_lighting(scene, &ray, reflected_dir, mat, o_hdr);
}

static void	material_specular(const t_scene *scene, const void *material_data,
	const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	const t_material_specular	*specular = material_data;
	t_vec3 reflected_dir;

	(void)scene;
	(void)material_data;
	(void)trace_data;
	(void)specular;

	float dot = vec3_dot(&trace_data->in_ray->direction, &trace_data->hit->normal);
	vec3_scale(&reflected_dir, &trace_data->hit->normal, dot * -2);
	vec3_add(&reflected_dir, &trace_data->in_ray->direction, &reflected_dir);

	*o_hdr = (t_color_hdr){ 0, 0, 0 };
	add_specular_lighting(scene, &trace_data->hit->location, &reflected_dir, specular, o_hdr);

	o_hdr->r *= specular->color.r;
	o_hdr->g *= specular->color.g;
	o_hdr->b *= specular->color.b;
}

bool	material_specular_init(t_material *material, const t_color_hdr *color, float pow)
{
	t_material_specular	*specular;

	specular = malloc(sizeof(t_material_specular));
	if (specular == NULL)
		return (false);
	specular->color = *color;
	specular->pow = pow;
	*material = (t_material)
	{
		.material_data = specular,
		.material_func = material_specular,
		.material_free_data_func = free
	};
	return (true);
}
