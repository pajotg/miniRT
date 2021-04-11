/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   renderer_init.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:21:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:46:22 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
//#include <stdlib.h>
//#include "ft_error.h"
//#include "libft.h"
//#include "ft_init.h"
#include "ft_init_create_threading.h"

#define ERR_MSG "Could not init renderer!"

// Really happy that i made the init_data function

static void	fill_init_data(t_init_data *init_data, t_mlx_data *data)
{
	init_data[0] = init_data_create_mutex(&data->renderer.start_thread_lock,
			ERR_MSG);
	init_data[1] = init_data_create_mutex(&data->renderer.hook_thread_lock,
			ERR_MSG);
	init_data[2] = init_data_create_mutex(&data
			->renderer.active_render_threads_lock, ERR_MSG);
	init_data[3] = init_data_create_mre(&data->renderer.no_render_threads_mre,
			ERR_MSG);
	init_data[4] = init_data_create_mre(&data->renderer.rendering_done_mre,
			ERR_MSG);
	init_data[5] = init_data_create_mutex(&data->renderer.start_thread_lock,
			ERR_MSG);
	init_data[6] = init_data_create_malloc(&data->renderer.pixels, sizeof(
				t_pixel_data) * data->scene->resolution.width * data->scene
			->resolution.height, ERR_MSG);
	init_data[7] = init_data_create_malloc(&data->renderer.temp_pixels, sizeof(
				t_temp_pixel_data) * data->scene->resolution.width * data
			->scene->resolution.height, ERR_MSG);
}

// *temp_pix = *pix;
//t_temp_pixel_data* temp_pix = &data->renderer.temp_pixels[i];

bool	renderer_init(t_mlx_data *data)
{
	int				i;
	t_init_data		init_data[8];
	t_pixel_data	*pix;

	fill_init_data(init_data, data);
	if (init_values(8, init_data) < 0)
		return (false);
	manual_reset_event_set(&data->renderer.rendering_done_mre);
	data->renderer.current_pixel = 0;
	data->renderer.active_render_threads = 0;
	data->renderer.frame_num = 1;
	i = 0;
	while (i < data->scene->resolution.width * data->scene->resolution.height)
	{
		pix = &data->renderer.pixels[i];
		pix->color = (t_color_hdr){0, 0, 0 };
		pix->num_samples = 0;
		i++;
	}
	update_image(data);
	return (true);
}
