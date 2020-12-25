/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:22 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/25 11:47:13 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include "mini_rt_parse_utils.h"
#include "mini_rt_utils.h"
#include "mini_rt_color_math_utils.h"
#include "mini_rt_mlx.h"
#include "mini_rt_objects.h"
#include "mini_rt_raytracing.h"
#include <stdbool.h>

typedef struct	s_args
{
	char* map_file;
	bool save;
}				t_args;

void init_ft_printf();

#endif