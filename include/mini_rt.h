/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/04 13:19:30 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
# define MINI_RT_H

# include "mini_rt_parse_utils.h"
# include "mini_rt_color_math_utils.h"
# include "mini_rt_mlx.h"
# include "mini_rt_objects.h"
# include "mini_rt_raytracing.h"
# include "mini_rt_hooks.h"
# include <stdbool.h>

void			init_ft_printf();
bool			write_bmp(int fd, unsigned char *pixels, int width, int height);

#endif
