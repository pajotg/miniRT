/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pix_to_ray.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:30:20 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:12:32 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_object.h"
#include <math.h>

/*
** Dont need to search up how, i can just use ALGEBRA!
** asin(0.5 / dist) = cam.fov / 2	// angle of ray = desired angle
** 0.5 / dist = sin(cam.fov / 2)
** 0.5 = sin(cam.fov / 2) * dist
** 0.5 / sin(cam.fov / 2) = dist
*/

/*
** dir = normalize(
**	  rot * { 1, 0, 0} * ox
** 	+ rot * { 0, 1, 0} * oy
** 	+ rot * { 0, 0,-1} * dist
** )
** this is also known as a rotation matrix multiplication
** and thats gets replaced by a rotation quaternion multiplication
*/

/*
** ox and oy are in the range -0.5 to 0.5
*/

void pix_to_ray(t_mlx_data* data, float x, float y, t_ray* o_ray)
{
	float fov_axis = data->scene->resolution.width;
	float ox = (data->scene->resolution.width / 2.0 - x) / fov_axis;
	float oy = (data->scene->resolution.height / 2.0 - y) / fov_axis;

	t_camera* cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	float dist = 0.5 / sin(cam->fov / 2);

	t_vec3 dir;
	t_vec3 local_dir = (t_vec3) { .x=ox, .y=oy, .z=-dist };
	vec3_normalize(&local_dir, &local_dir);
	quaternion_mult_vec3(&dir, &cam->transform.rotation, &local_dir);

	o_ray->origin = cam->transform.position;
	o_ray->direction = dir;
}