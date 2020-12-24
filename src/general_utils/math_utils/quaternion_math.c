/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quaternion_math.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:28:33 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/24 15:37:47 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_math_utils.h"
#include <math.h>

/*
**	Wikipedia!
**	Also: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/index.htm
**	the website says: x = i, y = j, z = k, w = r
*/

t_quaternion quaternion_new(float r, float i, float j, float k)
{
	t_quaternion quat;

	quat.r = r;
	quat.i = i;
	quat.j = j;
	quat.k = k;

	return (quat);
}

t_quaternion quaternion_from_AxisAngle(t_vec3 axis, float angle)
{
	float s = sinf(angle/2);
	float c = cosf(angle/2);
	return quaternion_new(
		c,
		axis.x * s,
		axis.y * s,
		axis.z * s
	);
}

t_quaternion quaternion_identity()
{
	return quaternion_new(1,0,0,0);
}

t_quaternion quaternion_mult(t_quaternion a, t_quaternion b)
{
	t_quaternion quat;

	quat.r = a.r * b.r - a.i * b.i - a.j * b.j - a.k * b.k;
	quat.i = a.r * b.i + a.i * b.r + a.j * b.k - a.k * b.j;
	quat.j = a.r * b.j - a.i * b.k + a.j * b.r + a.k * b.i;
	quat.k = a.r * b.k + a.i * b.j - a.j * b.i + a.k * b.r;

	return (quat);
}

t_quaternion quaternion_conjugate(t_quaternion a)
{
	return (quaternion_new(a.r, -a.i, -a.j, -a.k));
}

t_quaternion quaternion_normalize(t_quaternion a)
{
	float Magnitude = sqrtf(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
	return (quaternion_new(a.r / Magnitude, a.i / Magnitude, a.j / Magnitude, a.k / Magnitude));
}

/*
** This function can be easily optimized by expanding the equasions and removing the values that aren't needed
** kinda just discarding the result.x... /shrug
*/

t_vec3 quaternion_mult_vec3(t_quaternion a, t_vec3 b)
{
	t_quaternion result = quaternion_mult(quaternion_mult(a, quaternion_new(0, b.x, b.y, b.z)), quaternion_conjugate(a));
	return vec3_new(result.i, result.j, result.k);
}

t_quaternion quaternion_from_matrix(t_matrix3x3 matrix)
{
	t_quaternion quat;

	float tr = matrix.xx + matrix.yy + matrix.zz;

	if (tr > 0) {
		float S = sqrt(tr+1) * 2; // S=4*qw
		quat.r = 0.25 * S;
		quat.i = (matrix.zy - matrix.yz) / S;
		quat.j = (matrix.xz - matrix.zx) / S;
		quat.k = (matrix.yx - matrix.xy) / S;
	} else if ((matrix.xx > matrix.yy)&(matrix.xx > matrix.zz)) {
		float S = sqrtf(1 + matrix.xx - matrix.yy - matrix.zz) * 2; // S=4*qx
		quat.r = (matrix.zy - matrix.yz) / S;
		quat.i = 0.25 * S;
		quat.j = (matrix.xy + matrix.yx) / S;
		quat.k = (matrix.xz + matrix.zx) / S;
	} else if (matrix.yy > matrix.zz) {
		float S = sqrtf(1 + matrix.yy - matrix.xx - matrix.zz) * 2; // S=4*qy
		quat.r = (matrix.xz - matrix.zx) / S;
		quat.i = (matrix.xy + matrix.yx) / S;
		quat.j = 0.25 * S;
		quat.k = (matrix.yz + matrix.zy) / S;
	} else {
		float S = sqrtf(1 + matrix.zz - matrix.xx - matrix.yy) * 2; // S=4*qz
		quat.r = (matrix.yx - matrix.xy) / S;
		quat.i = (matrix.xz + matrix.zx) / S;
		quat.j = (matrix.yz + matrix.zy) / S;
		quat.k = 0.25 * S;
	}

	return quat;
}

/*
**	From the 2 vectors calculate the side axis
**	Now we have 3 vectors all at 90 degrees, AKA: a matrix
**	Now we can call quaternion_from_matrix to create a quaternion from it
*/

t_quaternion quaternion_from_forward_up(t_vec3 forward, t_vec3 up)
{
	t_vec3 side = vec3_cross(up, forward);
	up = vec3_cross(forward, side);

	t_matrix3x3 matrix;
	matrix.xx = side.x;
	matrix.xy = side.y;
	matrix.xz = side.z;

	matrix.yx = up.x;
	matrix.yy = up.y;
	matrix.yz = up.z;

	matrix.zx = forward.x;
	matrix.zy = forward.y;
	matrix.zz = forward.z;

	return quaternion_from_matrix(matrix);
}