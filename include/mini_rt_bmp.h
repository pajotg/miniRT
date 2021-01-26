/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_bmp.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:14:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:58:38 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_BMP_H
# define MINI_RT_BMP_H

# include "mini_rt_mlx.h"

bool	save_image(t_mlx_image *img, char *path);
bool	write_bmp(int fd, unsigned char *pixels, int width, int height);

#endif
