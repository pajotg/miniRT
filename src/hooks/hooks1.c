/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks1.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/16 17:29:33 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/25 14:30:10 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "ft_time.h"
#include <stdio.h>
#include "mini_rt_render_loop.h"
#include "mlx.h"

static t_time g_start_time;

void hook_frame_start(t_mlx_data *data)
{
	g_start_time = time_now();
	(void)data;
}

// Warning: can be called from multiple threads at once
void hook_frame_complete(t_mlx_data *data, bool first_frame)
{
	update_image(data);

	// Print out how long it took
	t_time current = time_now();
	float diff = time_difference(&current, &g_start_time);
	if (first_frame)
		printf("Completed first frame! time taken: %.2fs \n", diff);
	else
	{
		printf("Completed AA frame! time taken: %.2fs \n", diff);
		if (data->args->save)
		{
			correct_exit(data);	// call first, so the rendering stops
			save_image(&data->img, "screenshot.bmp");
		}
	}
}