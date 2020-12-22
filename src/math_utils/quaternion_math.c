/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quaternion_math.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:28:33 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 17:39:32 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_math_utils.h"
#include <math.h>

/*
**	Wikipedia!
**	Also: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/index.htm
*/

t_quaternion quaternion_new(float x, float y, float z, float w)
{
	t_quaternion quat;

	quat.x = x;
	quat.y = y;
	quat.z = z;
	quat.w = w;

	return (quat);
}

t_quaternion quaternion_from_AxisAngle(t_vec3 axis, float angle)
{
	float s = sinf(angle/2);
	float c = cosf(angle/2);
	return quaternion_new(
		axis.x * s,
		axis.y * s,
		axis.z * s,
		c
	);
}

t_quaternion quaternion_identity()
{
	return quaternion_new(1,0,0,0);
}

t_quaternion quaternion_mult(t_quaternion a, t_quaternion b)
{
	t_quaternion quat;

	quat.x = a.x * b.x - a.y * b.y - a.z * b.z - a.w * b.w;
	quat.y = a.x * b.y + a.y * b.x + a.z * b.w - a.w * b.z;
	quat.z = a.x * b.z - a.y * b.w + a.z * b.x + a.w * b.y;
	quat.w = a.x * b.w + a.y * b.z + a.z * b.y + a.w * b.x;

	return (quat);
}

t_quaternion quaternion_conjugate(t_quaternion a)
{
	return (quaternion_new(-a.x, -a.y, -a.z, a.w));
}

t_quaternion quaternion_normalize(t_quaternion a)
{
	float Magnitude = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	return (quaternion_new(a.x / Magnitude, a.y / Magnitude, a.z / Magnitude, a.w / Magnitude));
}

t_vec3 quaternion_mult_vec3(t_quaternion a, t_vec3 b)
{
	t_vec3 vec;

	float ww = a.w * a.w;
	float xx = a.x * a.x;
	float yy = a.y * a.y;
	float zz = a.z * a.z;
	float wx = a.w * a.x;
	float wy = a.w * a.y;
	float wz = a.w * a.z;
	float xy = a.x * a.y;
	float xz = a.x * a.z;
	float yz = a.y * a.z;

	// Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
	vec.x = ww*b.x + 2*wy*b.z - 2*wz*b.y +
				xx*b.x + 2*xy*b.y + 2*xz*b.z -
				zz*b.x - yy*b.x;
	vec.x = 2*xy*b.x + yy*b.y + 2*yz*b.z +
				2*wz*b.x - zz*b.y + ww*b.y -
				2*wx*b.z - xx*b.y;
	vec.x = 2*xz*b.x + 2*yz*b.y + zz*b.z -
				2*wy*b.x - yy*b.z + 2*wx*b.y -
				xx*b.z + ww*b.z;

	return (vec);
}