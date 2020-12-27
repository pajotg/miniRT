/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_math.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:49 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/27 15:37:22 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_math_utils.h"
#include <math.h>

const t_vec3* vec3_left()
{
	static const t_vec3 vec = { -1, 0, 0 };
	return &vec;
}

const t_vec3* vec3_right()
{
	static const t_vec3 vec = { 1, 0, 0 };
	return &vec;
}

const t_vec3* vec3_up()
{
	static const t_vec3 vec = { 0, 1, 0 };
	return &vec;
}

const t_vec3* vec3_down()
{
	static const t_vec3 vec = { 0, -1, 0 };
	return &vec;
}

const t_vec3* vec3_forward()
{
	static const t_vec3 vec = { 0, 0, -1 };
	return &vec;
}

const t_vec3* vec3_back()
{
	static const t_vec3 vec = { 0, 0, 1 };
	return &vec;
}

void vec3_init(t_vec3 *result, float x, float y, float z)
{
	result->x = x;
	result->y = y;
	result->z = z;
}

void vec3_clone(t_vec3 *result, const t_vec3 *vec)
{
	result->x = vec->x;
	result->y = vec->y;
	result->z = vec->z;
}

void vec3_add(t_vec3 *result, const t_vec3 *a, const t_vec3 *b)
{
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	result->z = a->z + b->z;
}

void vec3_subtract(t_vec3 *result, const t_vec3 *a, const t_vec3 *b)
{
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	result->z = a->z - b->z;
}

void vec3_multiply(t_vec3 *result, const t_vec3 *a, const t_vec3 *b)
{
	result->x = a->x * b->x;
	result->y = a->y * b->y;
	result->z = a->z * b->z;
}

void vec3_divide(t_vec3 *result, const t_vec3 *a, const t_vec3 *b)
{
	result->x = a->x / b->x;
	result->y = a->y / b->y;
	result->z = a->z / b->z;
}

void vec3_scale(t_vec3 *result, const t_vec3 *a, float scale)
{
	result->x = a->x * scale;
	result->y = a->y * scale;
	result->z = a->z * scale;
}

float vec3_magnitude_sqr(const t_vec3 *a)
{
	return (a->x * a->x + a->y * a->y + a->z * a->z);
}

float vec3_magnitude(const t_vec3 *a)
{
	return (sqrtf(vec3_magnitude_sqr(a)));
}

void vec3_normalize(t_vec3 *result, const t_vec3 *a)
{
	vec3_scale(result, a, 1/vec3_magnitude(a));
}

float vec3_dot(const t_vec3 *a, const t_vec3 *b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

void vec3_cross(t_vec3 *result, const t_vec3 *a, const t_vec3 *b)
{
	if (result == a || result == b)
	{
		t_vec3 temp;
		vec3_cross(&temp, a, b);
		vec3_init(result, temp.x, temp.y, temp.z);
		return;
	}
	result->x = a->y * b->z - a->z * b->y;
	result->y = a->z * b->x - a->x * b->z;
	result->z = a->x * b->y - a->y * b->x;
}
