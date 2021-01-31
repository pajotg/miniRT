/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks_frame.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/16 17:29:33 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/31 14:41:35 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_time.h"
#include "mini_rt_mlx.h"
#include "ft_printf.h"
#include "mini_rt_bmp.h"
#include <stdio.h>

static t_time g_start_time;

void hook_frame_start(t_mlx_data *data)
{
	g_start_time = time_now();
	(void)data;
}

void hook_frame_complete(t_mlx_data *data, int total_samples, float avg_noise)
{
	update_image(data);

	// Print out how long it took
	t_time current = time_now();
	float diff = time_difference(&current, &g_start_time);
	if (is_first_frame(&data->renderer))
		if (data->renderer.frame_num == 0)
			printf("Completed dirty frame! time taken: %.2fs, samples: %i\n", diff, total_samples);
		else
			printf("Completed first frame! time taken: %.2fs, samples: %i\n", diff, total_samples);
	else if (get_aa_frame(&data->renderer, data->scene) != -1)
		printf("Completed AA frame! time taken: %.2fs, samples: %i, avg noise: %.8f\n", diff, total_samples, avg_noise);
	else
		printf("Completed NR frame! time taken: %.2fs, samples: %i, avg noise: %.8f\n", diff, total_samples, avg_noise);

	// Check if we should save the frame
	if ( data->args->save && (
		(avg_noise < 0.00035 && !is_first_frame(&data->renderer) && get_aa_frame(&data->renderer, data->scene) == -1) ||
		(!data->renderer.rendering_done_mre.is_set)
	) )
	{
			correct_exit(data);	// call first, so the rendering stops
			save_image(&data->img, "screenshot.bmp");
	}
}