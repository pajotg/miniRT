/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_math.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:49 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 16:36:34 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_math_utils.h"
#include <math.h>

t_vec3 vec3_new(float x, float y, float z)
{
	t_vec3 vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;

	return (vec);
}

t_vec3 vec3_add(t_vec3 a, t_vec3 b)
{
	return (vec3_new(a.x + b.x, a.y + b.y, a.z + b.z));
}

t_vec3 vec3_subtract(t_vec3 a, t_vec3 b)
{
	return (vec3_new(a.x - b.x, a.y - b.y, a.z - b.z));
}

t_vec3 vec3_multiply(t_vec3 a, t_vec3 b)
{
	return (vec3_new(a.x * b.x, a.y * b.y, a.z * b.z));
}

t_vec3 vec3_divide(t_vec3 a, t_vec3 b)
{
	return (vec3_new(a.x / b.x, a.y / b.y, a.z / b.z));
}

t_vec3 vec3_scale(t_vec3 a, float b)
{
	return (vec3_new(a.x * b, a.y * b, a.z * b));
}

float vec3_magnitude_sqr(t_vec3 a)
{
	return (a.x * a.x + a.y * a.y + a.z * a.z);
}

float vec3_magnitude(t_vec3 a)
{
	return (sqrtf(vec3_magnitude_sqr(a)));
}

t_vec3 vec3_normalize(t_vec3 a)
{
	return vec3_scale(a, 1/vec3_magnitude(a));
}

float vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3 vec3_cross(t_vec3 a, t_vec3 b)
{
	return (vec3_new(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	));
}