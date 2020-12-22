/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   miniRT_math_utils.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:09:49 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 17:55:56 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_MATH_UTILS_H
# define MINIRT_MATH_UTILS_H

typedef struct	s_vec3
{
	float x;
	float y;
	float z;
}				t_vec3;

typedef struct	s_quaternion
{
	float x;
	float y;
	float z;
	float w;
}				t_quaternion;

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quaternion_math.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:28:33 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 17:34:51 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_math_utils.h"
#include <math.h>

/*
**	Vector functions
*/

t_vec3 vec3_new(float x, float y, float z);
t_vec3 vec3_add(t_vec3 a, t_vec3 b);
t_vec3 vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3 vec3_multiply(t_vec3 a, t_vec3 b);
float vec3_magnitude_sqr(t_vec3 a);
float vec3_magnitude(t_vec3 a);
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

#endif