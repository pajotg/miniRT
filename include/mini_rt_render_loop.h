/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_render_loop.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 14:01:15 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/17 14:08:23 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RENDER_LOOP_H
# define MINI_RT_RENDER_LOOP_H

# include "mini_rt_mlx.h"

void trace_pixel(t_mlx_data* data, int x, int y);
void trace_next_pixels(t_mlx_data* data, int desired);
void* new_pixel_render_thread(void* p);

void update_image(t_mlx_data* data);

#endif