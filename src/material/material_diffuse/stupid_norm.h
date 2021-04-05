#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_material_data.h"
# include "mini_rt_object.h"
# include <stdlib.h>
# include <math.h>
# include "mini_rt_render_pixel.h"
# include "libft.h"

# define NORMAL_OFFSET 0.001

typedef struct s_struct_abuse
{
	size_t		i;
	float		strength;
	float		sqr_dist;
	t_vec3		offset;
	t_vec3		normalized;
	t_light		*light;
	t_ray_hit	hit;
}	t_struct_abuse;

void	add_diffuse_lighting(const t_scene *scene, t_vec3 *position,
			const t_vec3 *normal, t_color_hdr *o_hdr);

#endif