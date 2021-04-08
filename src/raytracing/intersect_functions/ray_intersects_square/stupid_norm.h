#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_object.h"
# include "mini_rt_object_data.h"

typedef struct s_square_intersection_data
{
	t_vec3			normal;
	t_vec3			temp2;
	t_vec3			temp;
	float			height;
	float			travel_distance;
	float			dot;
	t_vec3			side;
	t_object_square	*data;
}	t_square_intersection_data;

#endif