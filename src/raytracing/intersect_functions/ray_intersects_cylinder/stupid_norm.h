#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_object.h"
# include "mini_rt_object_data.h"
# include <math.h>

# include <stdio.h>
# include <stdlib.h>

typedef struct s_cylinder_intersection_data
{
	float				dir2d_sqr;
	float				t;
	float				dsqr;
	float				k;
	bool				is_inside;
	float				dist;
	float				out_z;
	t_quaternion		conj;
	t_vec3				origin;
	t_vec3				dir;
	t_object_cylinder	*data;
}	t_cylinder_intersection_data;

typedef struct s_cylinder_caps_data
{
	bool	Above;
	float	Plane_z;
	float	Dist;
	float	OutX;
	float	OutY;
	float	SqrMag;
}	t_cylinder_caps_data;

typedef struct s_cylinder_caps_args
{
	float			Height;
	float			Radius;
	const t_vec3	*origin;
	const t_vec3	*dir;
	t_ray_hit		*o_hit;
}	t_cylinder_caps_args;

bool	ric_ray_intersects_cylinder_top_and_bottom(t_cylinder_caps_args cca);
void	ric_calculate_local_space(t_cylinder_intersection_data *cid,
			const t_object *object, const t_ray *ray);

#endif