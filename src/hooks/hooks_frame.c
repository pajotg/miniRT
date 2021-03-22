/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks_frame.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/16 17:29:33 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/20 15:16:53 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_time.h"
#include "mini_rt_mlx.h"
#include "ft_printf.h"
#include "mini_rt_bmp.h"
#include "mini_rt_color.h"
#include "mini_rt_pixel.h"
#include <stdio.h>

static t_time	g_start_time;

void	hook_frame_start(t_mlx_data *data)
{
	g_start_time = time_now();
	(void)data;
}

// Print out how long it took

static void	print_frame_data(t_mlx_data *data, int total_samples,
	float avg_noise)
{
	t_time	current;
	float	diff;

	current = time_now();
	diff = time_difference(&current, &g_start_time);
	if (is_first_frame(&data->renderer))
		if (data->renderer.frame_num == 0)
			printf("Completed DI frame! time taken: %.2fs, samples: %i\r", diff,
				total_samples);
	else
		printf("Completed FI frame! time taken: %.2fs, samples: %i\r", diff,
			total_samples);
	else if (get_aa_frame(&data->renderer, data->scene) != -1)
		printf("Completed AA frame! time taken: %.2fs, samples: %i, avg noise: "
			"%.8f\r", diff, total_samples, avg_noise);
	else
		printf("Completed NR frame! time taken: %.2fs, samples: %i, avg noise: "
			"%.8f\r", diff, total_samples, avg_noise);
	fflush(stdout);
}

// Check if we should save the frame
// call first, so the rendering stops

// Also, replacement for ternary, totaly better, yes, understandable.
//	Before: data->scene->use_gi ? 0.0035 : 0.000035;
//	After: data->scene->use_gi * 0.0035 + (!data->scene->use_gi) * 0.000035;
// 10/10 would norm again, such clean code, much wow, readable yes, goood

void	hook_frame_complete(t_mlx_data *data, int total_samples,
	float avg_noise)
{
	update_image(data);
	print_frame_data(data, total_samples, avg_noise);
	if (data->args->save && (
			(avg_noise < (data->scene->use_gi * 0.0035 + (!data->scene->use_gi)
					* 0.000035) && !is_first_frame(&data->renderer)
				&& get_aa_frame(&data->renderer, data->scene) == -1)
			|| (!data->renderer.rendering_done_mre.is_set)
			))
	{
		correct_exit(data);
		save_image(&data->img, "screenshot.bmp");
	}
}
