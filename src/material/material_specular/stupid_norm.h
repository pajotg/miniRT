#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_material_data.h"
# include "mini_rt_object.h"
# include <stdlib.h>
# include <math.h>
# include "mini_rt_render_pixel.h"
# include "libft.h"

# define NORMAL_OFFSET 0.001

// I did not feel like finding a way to do something like this without struct
//abuse
// I mean, how are you supposed to split up a function that does a simple
//thing, but needs many variables?
// Its just gonna get super ugly, way worse than this

typedef struct s_lighting_data
{
	const t_scene				*scene;
	const t_vec3				*position;
	const t_vec3				*reflected_dir;
	const t_material_specular	*mat;
	t_color_hdr					*o_hdr;
}	t_lighting_data;

typedef struct s_add_lighting_data
{
	const t_scene				*scene;
	t_ray						*ray;
	const t_vec3				*reflected_dir;
	const t_material_specular	*mat;
	t_color_hdr					*o_hdr;
}	t_add_lighting_data;

typedef struct s_struct_abse
{
	t_light		*light;
	t_vec3		offset;
	t_vec3		normalized;
	float		strength;
	float		sqr_dist;
	t_ray_hit	hit;
	size_t		i;
}	t_struct_abse;

void	material_specular(const t_scene *scene, const void *material_data,
			const t_material_trace_data *trace_data, t_color_hdr *o_hdr);

#endif