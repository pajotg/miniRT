/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   renderer_init.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:21:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 12:13:07 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include <stdlib.h>
#include "ft_error.h"
#include "libft.h"

bool	renderer_init(t_mlx_data* data)
{
	set_error("Could not init pthread mutex!",false);
	if (pthread_mutex_init(&data->renderer.start_thread_lock, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->renderer.hook_thread_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data->renderer.start_thread_lock);
		return (false);
	}
	if (pthread_mutex_init(&data->renderer.active_render_threads_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data->renderer.start_thread_lock);
		pthread_mutex_destroy(&data->renderer.hook_thread_lock);
		return (false);
	}
	if (manual_reset_event_init(&data->renderer.no_render_threads_mre) != 0)
	{
		pthread_mutex_destroy(&data->renderer.start_thread_lock);
		pthread_mutex_destroy(&data->renderer.hook_thread_lock);
		pthread_mutex_destroy(&data->renderer.active_render_threads_lock);
		return (false);
	}

	data->renderer.pixels = malloc(sizeof(t_pixel_data) * data->scene->resolution.width * data->scene->resolution.height);
	data->renderer.temp_pixels = malloc(sizeof(t_temp_pixel_data) * data->scene->resolution.width * data->scene->resolution.height);
	data->renderer.current_pixel = 0;
	data->renderer.active_render_threads = 0;
	data->renderer.frame_num = 1;

	if (data->renderer.pixels == NULL || data->renderer.temp_pixels == NULL)
	{
		free(data->renderer.pixels);
		free(data->renderer.temp_pixels);
		pthread_mutex_destroy(&data->renderer.start_thread_lock);
		pthread_mutex_destroy(&data->renderer.hook_thread_lock);
		pthread_mutex_destroy(&data->renderer.active_render_threads_lock);
		manual_reset_event_destroy(&data->renderer.no_render_threads_mre);
		set_error("Could not allocate pixel array!", false);
		return (false);
	}

	for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
	{
		t_pixel_data* pix = &data->renderer.pixels[i];
		//t_temp_pixel_data* temp_pix = &data->renderer.temp_pixels[i];
		pix->color = (t_color_hdr) { 0,0,0 };
		pix->num_samples = 0;
		//*temp_pix = *pix;
	}
	update_image(data);

	return (true);
}
