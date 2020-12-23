/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_color_math_utils.h                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:46:54 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 17:49:55 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_COLOR_MATH_UTILS_H
# define MINIRT_COLOR_MATH_UTILS_H

#include "mini_rt_math_utils.h"

typedef struct	s_color_hdr_to_rgb_jh
{
	float a;
	float b;
	float c;
	float d;
	float e;
	float f;
	float w;
}				t_color_hdr_to_rgb_jh;

t_color_rgb color_hdr_to_rgb_reindard(t_color_hdr hdr);
t_color_rgb color_hdr_to_rgb_reindard_white(t_color_hdr hdr, float white_point);
t_color_rgb color_hdr_to_rgb_jh(t_color_hdr hdr, t_color_hdr_to_rgb_jh* m);

#endif