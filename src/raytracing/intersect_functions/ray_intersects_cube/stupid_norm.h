#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_object.h"
# include "mini_rt_object_data.h"
# include <math.h>
# include "ft_vec3.h"

# include <stdlib.h>
# include <stdio.h>

typedef struct s_cube_intersection_data
{
	int				i;
	float			min_b[3];
	float			max_b[3];
	char			side[3];
	float			candidate_plane[3];
	bool			inside;
	float			coord;
	t_quaternion	conj;
	t_vec3			origin_vec;
	t_vec3			dir_vec;
	float			hit_t[3];
	t_object_cube	*data;
	float			*origin;
	float			*dir;
	int				which_plane;
}	t_cube_intersection_data;

void	ric_extract_data(t_cube_intersection_data *sa, const t_object *object,
			const t_ray *ray);
void	ric_calculate_sides(t_cube_intersection_data *sa);
void	ric_fix_inside(t_cube_intersection_data *sa);
void	ric_calculate_times(t_cube_intersection_data *sa);
void	ric_calculate_hit_plane(t_cube_intersection_data *sa);

#endif