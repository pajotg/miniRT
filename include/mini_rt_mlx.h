/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_mlx.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 18:51:02 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/21 14:13:42 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MLX_H
# define MINI_RT_MLX_H

# include "ft_manual_reset_event.h"
# include "mini_rt_math_utils.h"
# include "mini_rt_parse_utils.h"
# include <stdbool.h>

typedef struct	s_pixel_data
{
	t_color_hdr color;
	unsigned int num_samples;
}				t_pixel_data;

typedef struct	s_mlx_image
{
	void	*image;
	char	*addr;
	int		width;
	int		height;

	int		bits_per_pixel;
	int		line_length;
	bool	big_endian;
}				t_mlx_image;

typedef struct	s_input
{
	bool left;
	bool right;
	bool forward;
	bool backward;
	bool up;
	bool down;

	bool white_up;
	bool white_down;
}				t_input;


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

typedef struct	s_mlx_data
{
	void				*mlx;
	void				*window;
	t_scene				*scene;
	t_input				input;
	t_mlx_image			img;
	float				white;
	bool				should_clear;
	bool				active;
	bool				debug_trace_aabb;

	t_pixel_renderer	renderer;
}				t_mlx_data;

#endif
