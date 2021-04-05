/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pix_to_ray.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:30:20 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 17:42:17 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_object.h"
#include <math.h>
#include "stupid_norm.h"

/*
**	I guess im bad at math, this works atleast
**	tan(cam.fov / 2) = dy
**	ox and oy should be in the scale -1 to 1
**	ox and oy can be multiplied by dy, and z will be 1
*/

/*
** ox and oy are in the range -0.5 to 0.5
*/

//t_vec3 local_dir = (t_vec3) { .x=ox, .y=oy, .z=-dist };
//printf("dist: %.2f\n", dist);
//float dist = 0.5 / sin(cam->fov / 2);
//printf("ox: %.2f, oy: %.2f\n", ox, oy);

void	pix_to_ray(t_mlx_data *data, float x, float y, t_ray *o_ray)
{
	t_struct_abuse	sa;

	sa.fov_axis = data->scene->resolution.width;
	sa.ox = (data->scene->resolution.width / 2.0 - x) / sa.fov_axis;
	sa.oy = (data->scene->resolution.height / 2.0 - y) / sa.fov_axis;
	sa.cam = list_index(&data->scene->cameras, data->scene
			->current_camera_index);
	sa.dy = tanf(sa.cam->fov / 2) * 2;
	sa.local_dir = (t_vec3){.x = sa.ox * sa.dy, .y = sa.oy * sa.dy, .z = -1 };
	vec3_normalize(&sa.local_dir, &sa.local_dir);
	quaternion_mult_vec3(&sa.dir, &sa.cam->transform.rotation, &sa.local_dir);
	o_ray->origin = sa.cam->transform.position;
	o_ray->direction = sa.dir;
}
