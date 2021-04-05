#include "stupid_norm.h"

static void	add_point_lighting(const t_scene *scene, t_ray *ray, const t_vec3 *
	normal, t_color_hdr *o_hdr)
{
	t_struct_abuse	sa;

	sa.i = 0;
	while (sa.i < scene->lights.count)
	{
		sa.light = list_index(&scene->lights, sa.i);
		vec3_subtract(&sa.offset, &sa.light->position, &ray->origin);
		vec3_normalize(&sa.normalized, &sa.offset);
		sa.strength = vec3_dot(normal, &sa.normalized);
		if (sa.strength > 0)
		{
			sa.sqr_dist = vec3_magnitude_sqr(&sa.offset);
			ray->direction = sa.normalized;
			if (!cast_ray_max(scene, ray, &sa.hit, sqrtf(sa.sqr_dist)))
			{
				sa.strength *= 10 / sa.sqr_dist;
				o_hdr->r += sa.light->color.r * sa.strength;
				o_hdr->g += sa.light->color.g * sa.strength;
				o_hdr->b += sa.light->color.b * sa.strength;
			}
		}
		sa.i++;
	}
}

static void	add_directional_lighting(const t_scene *scene, t_ray *ray,
	const t_vec3 *normal, t_color_hdr *o_hdr)
{
	size_t				i;
	float				strength;
	t_directional_light	*light;
	t_ray_hit			hit;

	i = 0;
	while (i < scene->directional_lights.count)
	{
		light = list_index(&scene->directional_lights, i);
		strength = vec3_dot(normal, &light->direction);
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

void	add_diffuse_lighting(const t_scene *scene, t_vec3 *position,
	const t_vec3 *normal, t_color_hdr *o_hdr)
{
	t_ray	ray;

	vec3_scale(&ray.origin, normal, NORMAL_OFFSET);
	vec3_add(&ray.origin, position, &ray.origin);
	add_point_lighting(scene, &ray, normal, o_hdr);
	add_directional_lighting(scene, &ray, normal, o_hdr);
}
