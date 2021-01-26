/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_render_pixel.h                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:22:54 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:36:00 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RENDER_PIXEL_H
# define MINI_RT_RENDER_PIXEL_H

# include "mini_rt_mlx.h"
# include "mini_rt_raycast.h"

void	render_pixel(t_mlx_data *data, int x, int y);
void	render_next_pixels(t_mlx_data *data, int desired);

void	pix_to_ray(t_mlx_data *data, float x, float y, t_ray *o_ray);
void	trace_color(t_scene *scene, t_ray *ray, t_color_hdr *o_hdr);

void	*new_pixel_render_thread(void *p);

#endif
