#ifndef STUPID_NORM_H
# define STUPID_NORM_H

typedef struct s_struct_abuse
{
	float		fov_axis;
	float		ox;
	float		oy;
	float		dy;
	t_vec3		local_dir;
	t_vec3		dir;
	t_camera	*cam;
}	t_struct_abuse;

#endif