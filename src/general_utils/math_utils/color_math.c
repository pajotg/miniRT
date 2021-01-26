/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   color_math.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:38:38 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 18:09:34 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_color.h"
#include <math.h>

/*
**	A little more accurate, but slower
**		and more difficult to set a colors to a certain luminance
**	return sqrtf( 0.299*0.299*hdr.r*hdr.r
**				+ 0.587*0.587*hdr.g*hdr.g
**				+ 0.114*0.114*hdr.b*hdr.b
**			);
*/

float					to_luminance(t_color_hdr hdr)
{
	return (000.2126 * hdr.r
			+ 0.7152 * hdr.g
			+ 0.0722 * hdr.b);
}

static unsigned char	float_to_char(float f)
{
	if (f > 1)
		return (255);
	if (f < 0)
		return (0);
	return ((unsigned char)(f * 255));
}

t_color_rgb				color_hdr_to_rgb_reindard(t_color_hdr hdr)
{
	t_color_rgb	rgb;
	float		lum;
	float		scale;

	lum = to_luminance(hdr);
	scale = (lum / (lum + 1)) / lum;
	rgb.r = float_to_char(hdr.r * scale);
	rgb.g = float_to_char(hdr.g * scale);
	rgb.b = float_to_char(hdr.b * scale);
	return (rgb);
}

t_color_rgb				color_hdr_to_rgb_reindard_white(t_color_hdr hdr,
	float white_point)
{
	t_color_rgb	rgb;
	float		lum;
	float		new_lum;
	float		scale;

	lum = to_luminance(hdr);
	new_lum = lum * ((1 + (lum / (white_point * white_point))) / (1 + lum));
	scale = new_lum / lum;
	rgb.r = float_to_char(hdr.r * scale);
	rgb.g = float_to_char(hdr.g * scale);
	rgb.b = float_to_char(hdr.b * scale);
	return (rgb);
}

/*
**	I dont know exactly what function does, its just named t
**	so ill also name it "t"
**
**static float			t(float x, t_color_hdr_to_rgb_jh *m)
**{
**	return ((x * (m->a * x + m->c * m->b) + m->d * m->e)
**		/ (x * (m->a * x + m->b) + m->d * m->f) - m->e / m->f);
**}
**
**t_color_rgb				color_hdr_to_rgb_jh(t_color_hdr hdr,
**	t_color_hdr_to_rgb_jh *m)
**{
**	t_color_rgb	rgb;
**	float		lum;
**	float		scale;
**
**	lum = to_luminance(hdr);
**	scale = (t(lum * m->e, m) / t(m->w, m)) / lum;
**	rgb.r = float_to_char(hdr.r * scale);
**	rgb.g = float_to_char(hdr.g * scale);
**	rgb.b = float_to_char(hdr.b * scale);
**	return (rgb);
**}
*/
