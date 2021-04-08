#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_object.h"
# include "mini_rt_object_data.h"

typedef struct s_triangle_intersection_data
{
	float				a;
	float				f;
	float				u;
	float				v;
	float				distance;
	t_vec3				edge1;
	t_vec3				edge2;
	t_vec3				h;
	t_vec3				s;
	t_vec3				q;
	t_vec3				normal;
	t_object_triangle	*data;
}	t_triangle_intersection_data;

# define EPSILON 0.0000001

#endif