/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_math_utils.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:09:49 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 17:34:51 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_MATH_UTILS_H
# define MINIRT_MATH_UTILS_H

/*
**	Maybe i should make my functions take pointers, that is probably alot faster
**	Not passing all those arguments via the stack
*/

typedef struct	s_vec3
{
	float x;
	float y;
	float z;
}				t_vec3;

/*
**	I could have use unsigned chars, but i think having a HDR image is better
**	So why not? :)
*/

typedef struct	s_color_rgb
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}				t_color_rgb;

// To go from HDR to rgb we use tone mapping:
//	Reinhard tone mapping: c/(1+c), where c is a color channel (or luminance)
//	Reinhard with white point w: (1+c/w^2)/(1+c)
// I dont know, by John Hable, first used in uncharted 2
//	t(x) = (x*(ax + cb) + de) / (x*(ax + b) + df) - e/f
//	final color = t(ce) / t(w)	// e = exposure, w = white point, c = color, rest are constant values that configure the curve
// "Neutral": t( c/t(w) ) / t(w)
// ACES also exists

typedef struct	s_color_hdr
{
	float r;
	float g;
	float b;
}				t_color_hdr;

typedef struct	s_matrix3x3
{
	float xx;
	float yx;
	float zx;

	float xy;
	float yy;
	float zy;

	float xz;
	float yz;
	float zz;
}				t_matrix3x3;

typedef struct	s_quaternion
{
	float r;
	float i;
	float j;
	float k;
}				t_quaternion;

typedef struct	s_ray_hit
{
	t_vec3 location;
	t_vec3 normal;
	t_color_hdr color;
	float distance;
}				t_ray_hit;

typedef struct	s_ray
{
	t_vec3 origin;
	t_vec3 direction;
}				t_ray;

typedef struct	s_transform
{
	t_vec3 position;
	t_quaternion rotation;
}				t_transform;

typedef struct	s_camera
{
	t_transform transform;
	float fov;
}				t_camera;

typedef struct	s_light
{
	t_vec3 position;
	t_color_hdr color;
}				t_light;

/*
**	Vector functions
*/

t_vec3 vec3_new(float x, float y, float z);
t_vec3 vec3_add(t_vec3 a, t_vec3 b);
t_vec3 vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3 vec3_multiply(t_vec3 a, t_vec3 b);
t_vec3 vec3_divide(t_vec3 a, t_vec3 b);
t_vec3 vec3_scale(t_vec3 a, float b);
float vec3_magnitude_sqr(t_vec3 a);
float vec3_magnitude(t_vec3 a);
t_vec3 vec3_normalize(t_vec3 a);
float vec3_dot(t_vec3 a, t_vec3 b);
t_vec3 vec3_cross(t_vec3 a, t_vec3 b);

/*
**	Quaternion functions
*/

t_quaternion quaternion_new(float x, float y, float z, float w);
t_quaternion quaternion_from_AxisAngle(t_vec3 axis, float angle);
t_quaternion quaternion_identity();
t_quaternion quaternion_mult(t_quaternion a, t_quaternion b);
t_quaternion quaternion_conjugate(t_quaternion a);
t_quaternion quaternion_normalize(t_quaternion a);
t_vec3 quaternion_mult_vec3(t_quaternion a, t_vec3 b);
t_quaternion quaternion_from_matrix(t_matrix3x3 matrix);
t_quaternion quaternion_from_forward_up(t_vec3 forward, t_vec3 up);

#endif