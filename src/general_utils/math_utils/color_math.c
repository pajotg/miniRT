/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   color_math.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:38:38 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 18:00:14 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_color_math_utils.h"

static unsigned char float_to_char(float f)
{
	if (f > 1)
		return 255;
	if (f < 0)
		return 0;
	return (unsigned char)(f * 255);
}

t_color_rgb color_hdr_to_rgb_reindard(t_color_hdr hdr)
{
	t_color_rgb rgb;

	rgb.r = float_to_char(hdr.r / (hdr.r + 1));
	rgb.g = float_to_char(hdr.g / (hdr.g + 1));
	rgb.b = float_to_char(hdr.b / (hdr.b + 1));

	return rgb;
}

t_color_rgb color_hdr_to_rgb_reindard_white(t_color_hdr hdr, float white_point)
{
	t_color_rgb rgb;

	float sqr = white_point * white_point;

	rgb.r = float_to_char(hdr.r * ((1 + (hdr.r / sqr)) / (1 + hdr.r)));

	rgb.g = float_to_char(hdr.g * ((1 + (hdr.g / sqr)) / (1 + hdr.g)));

	rgb.b = float_to_char(hdr.b * ((1 + (hdr.b / sqr)) / (1 + hdr.b)));

	return rgb;
}

/*
**	I dont know exactly what function does, its just named t
**	so ill also name it "t"
*/

static float t(float x, t_color_hdr_to_rgb_jh* m)
{
	return (x*(m->a*x + m->c*m->b) + m->d*m->e) / (x*(m->a*x + m->b) + m->d*m->f) - m->e/m->f;
}

t_color_rgb color_hdr_to_rgb_jh(t_color_hdr hdr, t_color_hdr_to_rgb_jh* m)
{
	t_color_rgb rgb;

	rgb.r = float_to_char(t(hdr.r * m->e, m) / t(m->w, m));
	rgb.g = float_to_char(t(hdr.g * m->e, m) / t(m->w, m));
	rgb.b = float_to_char(t(hdr.b * m->e, m) / t(m->w, m));

	return rgb;
}