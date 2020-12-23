/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quaternion_math.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:28:33 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 21:47:50 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_math_utils.h"
#include <math.h>

/*
**	Wikipedia!
**	Also: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/index.htm
**	Well, euclideanspace has a different idea of what direction the x,y,z,w are
**	My directions are: x = 1, y = i, z = j, w = k
**	But the website says: x = i, y = j, z = k, w = 1
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

    quat.x = a.x * b.x - a.y * b.y - a.z * b.z - a.w * b.w;
	quat.y = a.x * b.y + a.y * b.x + a.z * b.w - a.w * b.z;
    quat.z = a.x * b.z - a.y * b.w + a.z * b.x + a.w * b.y;
    quat.w = a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x;

	return (quat);
}

t_quaternion quaternion_conjugate(t_quaternion a)
{
	return (quaternion_new(a.x, -a.y, -a.z, -a.w));
}

t_quaternion quaternion_normalize(t_quaternion a)
{
	float Magnitude = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	return (quaternion_new(a.x / Magnitude, a.y / Magnitude, a.z / Magnitude, a.w / Magnitude));
}

/*
** This function can be easily optimized by expanding the equasions and removing the values that aren't needed
** kinda just discarding the result.x... /shrug
*/

t_vec3 quaternion_mult_vec3(t_quaternion a, t_vec3 b)
{
	t_quaternion result = quaternion_mult(quaternion_mult(a, quaternion_new(0, b.x, b.y, b.z)), quaternion_conjugate(a));
	return vec3_new(result.y, result.z, result.w);
}

t_quaternion quaternion_from_matrix(t_matrix3x3 matrix)
{
	t_quaternion quat;

	float tr = matrix.xx + matrix.yy + matrix.zz;

	if (tr > 0) {
		float S = sqrt(tr+1) * 2; // S=4*qw
		quat.x = 0.25 * S;
		quat.y = (matrix.zy - matrix.yz) / S;
		quat.z = (matrix.xz - matrix.zx) / S;
		quat.w = (matrix.yx - matrix.xy) / S;
	} else if ((matrix.xx > matrix.yy)&(matrix.xx > matrix.zz)) {
		float S = sqrtf(1 + matrix.xx - matrix.yy - matrix.zz) * 2; // S=4*qx
		quat.x = (matrix.zy - matrix.yz) / S;
		quat.y = 0.25 * S;
		quat.z = (matrix.xy + matrix.yx) / S;
		quat.w = (matrix.xz + matrix.zx) / S;
	} else if (matrix.yy > matrix.zz) {
		float S = sqrtf(1 + matrix.yy - matrix.xx - matrix.zz) * 2; // S=4*qy
		quat.x = (matrix.xz - matrix.zx) / S;
		quat.y = (matrix.xy + matrix.yx) / S;
		quat.z = 0.25 * S;
		quat.w = (matrix.yz + matrix.zy) / S;
	} else {
		float S = sqrtf(1 + matrix.zz - matrix.xx - matrix.yy) * 2; // S=4*qz
		quat.x = (matrix.yx - matrix.xy) / S;
		quat.y = (matrix.xz + matrix.zx) / S;
		quat.z = (matrix.yz + matrix.zy) / S;
		quat.w = 0.25 * S;
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