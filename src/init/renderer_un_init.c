/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   renderer_un_init.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:21:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/27 17:49:31 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_render_pixel.h"
#include "ft_manual_reset_event.h"
#include <pthread.h>
#include <stdlib.h>


void	renderer_un_init(t_pixel_renderer* renderer)
{
	pthread_mutex_destroy(&renderer->start_thread_lock);
	pthread_mutex_destroy(&renderer->hook_thread_lock);
	pthread_mutex_destroy(&renderer->active_render_threads_lock);
	manual_reset_event_destroy(&renderer->no_render_threads_mre);
	free(renderer->pixels);
	free(renderer->temp_pixels);
}
