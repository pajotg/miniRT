/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_math_utils.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:09:49 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/15 21:53:47 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MATH_UTILS_H
# define MINI_RT_MATH_UTILS_H

# include "ft_quaternion.h"
# include "ft_aabb.h"

/*
**	I could have use unsigned chars, but i think having a HDR image is better
**	So why not? :)
*/

typedef struct		s_color_rgb
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
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

typedef struct		s_color_hdr
{
	float r;
	float g;
	float b;
}					t_color_hdr;

typedef struct		s_ray_hit
{
	t_vec3		location;
	t_vec3		normal;
	t_color_hdr	color;
	float		distance;
}					t_ray_hit;

typedef struct		s_ray
{
	t_vec3 origin;
	t_vec3 direction;
}					t_ray;

typedef struct		s_transform
{
	t_vec3			position;
	t_quaternion	rotation;
}					t_transform;

typedef struct		s_camera
{
	t_transform	transform;
	float		fov;
}					t_camera;

typedef struct		s_light
{
	t_vec3		position;
	t_color_hdr	color;
}					t_light;

#endif
