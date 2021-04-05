/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks_mlx_loop.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:28:12 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 17:33:51 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "ft_time.h"
#include "ft_manual_reset_event.h"
#include <stdio.h>
#include <math.h>
#include "mini_rt_object.h"
#include "mini_rt_render_pixel.h"
#include "mlx.h"
#include "ft_ternary.h"

#include "stupid_norm.h"

//	returns time since last frame
// 	Get time since last frame

static float	hook_frame(void)
{
	t_time			current;
	float			diff;
	static t_time	last = {0, 0 };

	current = time_now();
	diff = time_difference(&current, &last);
	if (last.seconds == 0)
		diff = 0;
	last = current;
	return (diff);
}

//printf("Got inputs: %i|%i %i|%i\n", data->input.forward,data->input.backward,
//
//data->input.left, data->input.right);

static void	handle_movement(t_mlx_data *data, float diff)
{
	t_vec3		move_dir;
	t_camera	*cam;

	move_dir = (t_vec3){0, 0, 0 };
	data->should_clear = data->should_clear || (data->input.forward != data
			->input.backward)
		|| (data->input.right != data->input.left)
		|| (data->input.down != data->input.up);
	move_dir.z = ter_int(data->input.forward, -1, 0) + ter_int(data
			->input.backward, 1, 0);
	move_dir.x = ter_int(data->input.right, -1, 0) + ter_int(data->input.left,
			1, 0);
	move_dir.y = ter_int(data->input.down, -1, 0) + ter_int(data->input.up, 1,
			0);
	cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	quaternion_mult_vec3(&move_dir, &cam->transform.rotation, &move_dir);
	vec3_scale(&move_dir, &move_dir, diff * 5);
	vec3_add(&cam->transform.position, &cam->transform.position, &move_dir);
}

// in case we have stopped rendering, start again
// Instead of resetting the progress of the frame, only getting updated pixels
//at the top, i mark this frame dirty
//data->renderer.current_pixel = 0;	// restart progress of frame, so we dont
//have stale pixels
// Wait untill all render threads have stopped
// If we have moved, we need to clear the pixels
// dirty frame
// Prevent new threads from starting

static void	handle_clearing(t_mlx_data *data)
{
	if (data->should_clear)
	{
		data->should_clear = false;
		pthread_mutex_lock(&data->renderer.start_thread_lock);
		manual_reset_event_wait(&data->renderer.no_render_threads_mre);
		if (data->renderer.rendering_done_mre.is_set)
			data->renderer.frame_num = 0;
		else
			data->renderer.frame_num = 1;
		manual_reset_event_set(&data->renderer.rendering_done_mre);
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
	}
}

static void	handle_screen_update(t_mlx_data *data)
{
	static int	last_frame = 0;

	if (data->window
		 && (last_frame != data->renderer.frame_num || (is_first_frame(&data
					->renderer) && data->renderer.rendering_done_mre.is_set)
			 || data
			->should_update_screen))
	{
		data->should_update_screen = false;
		last_frame = data->renderer.frame_num;
		mlx_put_image_to_window(data->mlx, data->window, data->img.image, 0, 0);
	}
}

//	Beware! if you get more than 400 fps it gets REAL LAGGY...
//	What?

int	hook_loop(void *p)
{
	float		diff;
	t_mlx_data	*data;

	data = p;
	if (!data->active)
		mlx_loop_on_exit(data);
	diff = hook_frame();
	handle_movement(data, diff);
	handle_clearing(data);
	render_next_pixels(data, 250);
	handle_white_point(data, diff);
	handle_screen_update(data);
	return (0);
}
