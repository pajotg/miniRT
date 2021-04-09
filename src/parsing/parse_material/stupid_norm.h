#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_material_data.h"
# include "ft_parse_utils.h"
# include "mini_rt_parse_utils.h"
# include "libft.h"
# include "ft_error.h"
# include <stdlib.h>

// TODO:
//	Effects: normal disruption
//	Color disruption and rainbow effect
//	Materials: Glossy and transparent

typedef struct s_mix_data
{
	float		ratio;
	t_shared_pt	*a;
	t_shared_pt	*b;
}	t_mix_data;

t_shared_pt	*create_shared_ptr_from_material(t_material *material);

t_shared_pt	*read_diffuse(const char *str, int *current);
t_shared_pt	*read_emissive(const char *str, int *current);
t_shared_pt	*read_perfect_mirror(const char *str, int *current);
t_shared_pt	*read_specular(const char *str, int *current);
t_shared_pt	*read_mix(const char *str, int *current);
t_shared_pt	*read_additive(const char *str, int *current);
t_shared_pt	*read_checkerboard(const char *str, int *current);

#endif