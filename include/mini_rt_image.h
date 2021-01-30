/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_image.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 18:17:40 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 14:22:15 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_IMAGE_H
# define MINI_RT_IMAGE_H

# include "mini_rt_mlx.h"

void	write_pix(t_mlx_data *data, int x, int y, t_color_hdr hdr);
void	update_pix(t_mlx_data *data, int x, int y);
void	update_image(t_mlx_data *data);
float	get_edgyness(t_mlx_data *data, int x, int y);
float	get_noisyness(t_mlx_data* data, int x, int y);

#endif
