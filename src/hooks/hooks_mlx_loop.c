/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks_mlx_loop.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:28:12 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 14:10:45 by jsimonis      ########   odam.nl         */
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

/*
**	returns time since last frame
*/

static float hook_frame()
{
	// 	Get time since last frame
	static t_time last = { 0, 0 };
	t_time current = time_now();
	float diff = time_difference(&current, &last);
	if (last.seconds == 0)
		diff = 0;
	last = current;

	//	Accumulate values
	/*
	static float time = 0;
	static int frames = 0;
	time += diff;
	frames++;
	//	Print out
	if (time > 1)
	{
		printf("%i fps (%.4fs/f)\n", frames, time / frames);
		time = 0;
		frames = 0;
	}
	*/
	return (diff);
}

/*
**	Beware! if you get more than 400 fps it gets REAL LAGGY...
**	What?
*/

int	hook_loop(void *p)
{
	t_mlx_data* data = p;

	float diff = hook_frame();

	// Input
	t_vec3 move_dir = (t_vec3) { 0, 0, 0 };
	data->should_clear = data->should_clear || (data->input.forward != data->input.backward)
					|| (data->input.right != data->input.left)
					|| (data->input.down != data->input.up);
	move_dir.z = (data->input.forward ? -1 : 0) + (data->input.backward ? 1 : 0);
	move_dir.x = (data->input.right ? -1 : 0) + (data->input.left ? 1 : 0);
	move_dir.y = (data->input.down ? -1 : 0) + (data->input.up ? 1 : 0);

	t_camera* cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	quaternion_mult_vec3(&move_dir, &cam->transform.rotation, &move_dir);
	vec3_scale(&move_dir, &move_dir, diff * 5);
	vec3_add(&cam->transform.position, &cam->transform.position, &move_dir);

	if (data->should_clear)
	{
		data->should_clear = false;

		// If we have moved, we need to clear the pixels
		pthread_mutex_lock(&data->renderer.start_thread_lock);	// Prevent new threads from starting
		manual_reset_event_wait(&data->renderer.no_render_threads_mre);	// Wait untill all render threads have stopped
		//data->renderer.current_pixel = 0;	// restart progress of frame, so we dont have stale pixels
		data->renderer.frame_num = 0;	// Instead of resetting the progress of the frame, only getting updated pixels at the top, i mark this frame dirty
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
	}
	// Render
	render_next_pixels(data, 250);

	bool should_update = false;
	if (data->input.white_up != data->input.white_down)
	{
		float change = powf(1.1,
			((data->input.white_down ? -1 : 0) + (data->input.white_up ? 1 : 0))
			* (diff * 2)
		);
		data->white *= change;
		if (data->white < 0.001)
			data->white = 0.001;
		update_image(data);
		should_update = true;
	}

	static int last_frame = 0;
	if (data->window || last_frame != data->renderer.frame_num || is_first_frame(&data->renderer) || should_update)
	{
		last_frame = data->renderer.frame_num;
		mlx_put_image_to_window(data->mlx, data->window, data->img.image, 0, 0);
	}

	return 0;
}