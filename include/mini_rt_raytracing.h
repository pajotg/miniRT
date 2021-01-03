/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_raytracing.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 10:50:51 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 11:35:27 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RAYTRACING_H
# define MINI_RT_RAYTRACING_H

# include "mini_rt_mlx.h"
# include "mini_rt_math_utils.h"

bool trace_ray(t_mlx_data *data, t_ray *ray, t_ray_hit *o_hit);
void trace_color(t_mlx_data *data, t_ray *ray, t_color_hdr *o_hdr);
void pix_to_ray(t_mlx_data *data, int x, int y, t_ray *o_ray);

#endif
