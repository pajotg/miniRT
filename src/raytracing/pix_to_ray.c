/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pix_to_ray.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:30:20 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/11 12:52:20 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_object.h"
#include <math.h>

/*
**	I guess im bad at math, this works atleast
**	tan(cam.fov / 2) = dy
**	ox and oy should be in the scale -1 to 1
**	ox and oy can be multiplied by dy, and z will be 1
*/

/*
** ox and oy are in the range -0.5 to 0.5
*/

void pix_to_ray(t_mlx_data* data, float x, float y, t_ray* o_ray)
{
	float fov_axis = data->scene->resolution.width;
	float ox = (data->scene->resolution.width / 2.0 - x) / fov_axis;
	float oy = (data->scene->resolution.height / 2.0 - y) / fov_axis;

	//printf("ox: %.2f, oy: %.2f\n", ox, oy);
	t_camera* cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	//float dist = 0.5 / sin(cam->fov / 2);
	float dy = tanf(cam->fov / 2) * 2;
	//printf("dist: %.2f\n", dist);

	t_vec3 dir;
	//t_vec3 local_dir = (t_vec3) { .x=ox, .y=oy, .z=-dist };
	t_vec3 local_dir = (t_vec3) { .x=ox * dy, .y=oy * dy, .z=-1 };
	vec3_normalize(&local_dir, &local_dir);
	quaternion_mult_vec3(&dir, &cam->transform.rotation, &local_dir);

	o_ray->origin = cam->transform.position;
	o_ray->direction = dir;
}