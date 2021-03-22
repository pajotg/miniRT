/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_init.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:25:10 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/27 17:42:19 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_INIT_H
# define MINI_RT_INIT_H

# include "mini_rt_mlx.h"

void	init_ft_printf(void);

bool	renderer_init(t_mlx_data *data);
void	renderer_un_init(t_pixel_renderer *renderer);

bool	mlx_data_init(t_mlx_data *data, void *mlx, t_scene *scene, t_args *args)
		;
void	mlx_data_un_init(t_mlx_data *data);

bool	image_init(void *mlx, t_mlx_image *img, int width, int height);
void	image_un_init(void *mlx, t_mlx_image *img);

#endif
