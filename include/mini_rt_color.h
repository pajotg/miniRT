/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_color.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:19:12 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/21 11:46:42 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_COLOR_H
# define MINI_RT_COLOR_H

/*
**	I could have use unsigned chars, but i think having a HDR image is better
**	So why not? :)
*/

typedef struct s_color_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_color_rgb;

/*
** To go from HDR to rgb we use tone mapping:
**	Reinhard tone mapping: c/(1+c), where c is a color channel (or luminance)
**	Reinhard with white point w: c*(1+c/w^2)/(1+c)
** I dont know, by John Hable, first used in uncharted 2
**	t(x) = (x*(ax + cb) + de) / (x*(ax + b) + df) - e/f
**	final color = t(ce) / t(w)
**		e = exposure, w = white point, c = color
**		the rest are constant values that configure the curve
** "Neutral": t( c/t(w) ) / t(w)
** ACES also exists
*/

typedef struct s_color_hdr
{
	float	r;
	float	g;
	float	b;
}					t_color_hdr;

/*
**typedef struct	s_color_hdr_to_rgb_jh
**{
**	float a;
**	float b;
**	float c;
**	float d;
**	float e;
**	float f;
**	float w;
**}					t_color_hdr_to_rgb_jh;
*/

float				color_hdr_to_luminance(t_color_hdr hdr);

t_color_rgb			color_hdr_to_rgb_reindard(t_color_hdr hdr);
t_color_rgb			color_hdr_to_rgb_reindard_white(t_color_hdr hdr,
						float white_point);

/*
**t_color_rgb color_hdr_to_rgb_jh(t_color_hdr hdr, t_color_hdr_to_rgb_jh* m);
*/

#endif
