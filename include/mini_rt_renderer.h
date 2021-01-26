/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_renderer.h                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:22:16 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:22:33 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RENDERER_H
# define MINI_RT_RENDERER_H

# include <pthread.h>
# include "ft_manual_reset_event.h"
# include "mini_rt_pixel.h"
# include "mini_rt_color.h"
# include "mini_rt_raycast.h"

/*
** 	When rendering normally:
**		lock start_thread_lock
**		do calculations to determine what pixels we render
**		unlock start_thread_lock
**		on start, if first render thread, reset no_render_threads_mre
**		on finish, if last render thread set no_render_threads_mre
**	When a frame finishes:
**		lock start_thread_lock
**		wait on no_render_threads_mre
**		do copying and sobel filter (first frame = set, else additive)
**		new_thread_mre is set
**		unlock start_thread_lock
**	When you move:
**		lock start_thread_lock
**		wait on no_render_threads_mre
**		set current_pixel to 0
**		set first_frame and dirty_frame to true
**		unlock start_thread_lock
*/

typedef struct	s_pixel_renderer
{
	pthread_mutex_t			active_render_threads_lock;
	int						active_render_threads;
	t_manual_reset_event	no_render_threads_mre;
	t_pixel_data			*pixels;

	pthread_mutex_t			hook_thread_lock;
	pthread_mutex_t			start_thread_lock;
	bool					first_frame;
	bool					dirty_frame;
	int						current_pixel;
	t_pixel_data			*temp_pixels;
}				t_pixel_renderer;

#endif
