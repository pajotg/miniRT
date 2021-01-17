/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/03 14:11:21 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/17 13:33:47 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define KEY_ESC 65307
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_Q 113
#define KEY_E 101
#define KEY_T 116
#define KEY_Z 122

#define KEY_R 114
#define KEY_F 102

#include "mini_rt_mlx.h"
#include "mini_rt_math_utils.h"
#include "mini_rt_raytracing.h"
#include "mlx.h"
#include "ft_printf.h"
#include <stdbool.h>
#include <math.h>

#include <stdio.h>	// bad

static void next_cam(t_mlx_data* data)
{
	size_t index;

	index = data->scene->current_camera_index + 1;
	if (index >= data->scene->cameras.count)
		index = 0;
	data->scene->current_camera_index = index;
}

int	hook_key_down(int key,void *p)
{
	t_mlx_data* data = p;
	//printf("Key down: %i\n", key);

	if (key == KEY_ESC)
		mlx_loop_end(data->mlx);
	else if (key == KEY_W)
		data->input.forward = true;
	else if (key == KEY_A)
		data->input.left = true;
	else if (key == KEY_S)
		data->input.backward = true;
	else if (key == KEY_D)
		data->input.right = true;
	else if (key == KEY_Q)
		data->input.up = true;
	else if (key == KEY_E)
		data->input.down = true;
	else if (key == KEY_R)
		data->input.white_up = true;
	else if (key == KEY_F)
		data->input.white_down = true;
	else if (key == KEY_Z)
		data->debug_trace_aabb = !data->debug_trace_aabb;
	else if (key == KEY_T)
		next_cam(data);
	return 0;
}

int	hook_key_up(int key,void *p)
{
	t_mlx_data* data = p;
	//printf("Key up: %i\n", key);

	if (key == KEY_W)
		data->input.forward = false;
	else if (key == KEY_A)
		data->input.left = false;
	else if (key == KEY_S)
		data->input.backward = false;
	else if (key == KEY_D)
		data->input.right = false;
	else if (key == KEY_Q)
		data->input.up = false;
	else if (key == KEY_E)
		data->input.down = false;
	else if (key == KEY_R)
		data->input.white_up = false;
	else if (key == KEY_F)
		data->input.white_down = false;
	return 0;
}

/*
**	1 (left click) = look at point
**	2 (middle click) = gather debug data from pixel
**	3 (right click) = get HDR color
*/

int hook_mouse(int button, int x, int y, void* p)
{
	t_mlx_data* data = p;

	//printf("mouse hooked! %i at: (%i, %i) %p\n",button, x, y, p);
	if (button == 1)
	{
		t_ray ray;
		pix_to_ray(data, x, y, &ray);

		t_camera* cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
		float sqrmag = vec3_magnitude_sqr(&ray.direction);
		if (sqrmag < 0.99 || sqrmag > 1.01)
		{
			printf("Bad ray create! %.2f = sqrmag != 1: %.2f %.2f %.2f\n", sqrmag, ray.direction.x, ray.direction.y, ray.direction.z);
		}
		t_quaternion new_rot;
		quaternion_from_forward_up(&new_rot, &ray.direction, vec3_up());
		cam->transform.rotation = new_rot;
	}
	if (button == 2)
	{
		t_ray ray;
		t_ray_hit hit;

		pix_to_ray(data, x, y, &ray);
		if (trace_ray(data, &ray, &hit))
		{
			ft_printf("Hit!\n");
			ft_printf("	Location: %v!\n", &hit.location);
			printf("	Distance: %.2f!\n", hit.distance);
			ft_printf("	Normal: %v!\n", &hit.normal);
			ft_printf("	Color: %v!\n", (t_vec3*)&hit.color);

			float sqrmag = vec3_magnitude_sqr(&hit.normal);
			if (sqrmag > 1.01 || sqrmag < 0.99)
				printf("		Err: Normal magnitude != 1, actual: %.2f\n", sqrtf(sqrmag));

			t_vec3 expected_loc;
			vec3_scale(&expected_loc, &ray.direction, hit.distance);
			vec3_add(&expected_loc, &expected_loc, &ray.origin);

			t_vec3 diff;
			vec3_subtract(&diff, &expected_loc, &hit.location);
			sqrmag = vec3_magnitude_sqr(&diff);
			if (sqrmag > 0.01)
				ft_printf("		Err: Expected location %v but got: %v!\n", &expected_loc, &hit.location);

			if (vec3_dot(&hit.normal, &ray.direction) > 0)
				ft_printf("		Err: hit.normal in opposite direction!\n");
		}
		else
			printf("Miss!\n");
	}
	else if (button == 3)
	{
		t_color_hdr hdr = data->pixels[x + y * data->scene->resolution.width].color;
		printf("Color: %.2f %.2f %.2f, mag: %.2f\n", hdr.r, hdr.g, hdr.b,
			sqrtf(hdr.r * hdr.r + hdr.g * hdr.g + hdr.b * hdr.b)
		);
	}
	return 0;
}

int hook_client_message(void* p)
{
	t_mlx_data* data = p;
	mlx_loop_end(data->mlx);
	return 0;
}