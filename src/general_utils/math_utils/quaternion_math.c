/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quaternion_math.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:28:33 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/27 15:42:38 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_math_utils.h"
#include <math.h>

/*
**	Wikipedia!
**	Also: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/index.htm
**	the website says: x = i, y = j, z = k, w = r
*/

const t_quaternion* quaternion_identity()
{
	static const t_quaternion quat = { 1, 0, 0, 0 };
	return &quat;
}

void quaternion_init(t_quaternion *result, float r, float i, float j, float k)
{
	result->r = r;
	result->i = i;
	result->j = j;
	result->k = k;
}

void quaternion_from_AxisAngle(t_quaternion *result, const t_vec3 *axis, const float angle)
{
	float s = sinf(angle/2);
	float c = cosf(angle/2);
	return quaternion_init(
		result,
		c,
		axis->x * s,
		axis->y * s,
		axis->z * s
	);
}

void quaternion_mult(t_quaternion *result, const t_quaternion *a, const t_quaternion *b)
{
	if (result == a || result == b)
	{
		t_quaternion temp;
		quaternion_mult(&temp, a, b);
		quaternion_init(result, temp.r, temp.i, temp.j, temp.k);
		return;
	}
	result->r = a->r * b->r - a->i * b->i - a->j * b->j - a->k * b->k;
	result->i = a->r * b->i + a->i * b->r + a->j * b->k - a->k * b->j;
	result->j = a->r * b->j - a->i * b->k + a->j * b->r + a->k * b->i;
	result->k = a->r * b->k + a->i * b->j - a->j * b->i + a->k * b->r;
}

void quaternion_conjugate(t_quaternion *result, const t_quaternion *a)
{
	quaternion_init(result, a->r, -a->i, -a->j, -a->k);
}

void quaternion_normalize(t_quaternion *result, const t_quaternion *a)
{
	float Magnitude = sqrtf(a->r * a->r + a->i * a->i + a->j * a->j + a->k * a->k);
	quaternion_init(result, a->r / Magnitude, a->i / Magnitude, a->j / Magnitude, a->k / Magnitude);
}

/*
** This function can be easily optimized by expanding the equation and removing the values that aren't needed
** kinda just discarding the temp.x... /shrug
*/

void quaternion_mult_vec3(t_vec3 *result, const t_quaternion *a, const t_vec3 *vec)
{
	t_quaternion temp;
	t_quaternion conjugate;

	quaternion_init(&temp, 0, vec->x, vec->y, vec->z);
	quaternion_mult(&temp, a, &temp);
	quaternion_conjugate(&conjugate, a);
	quaternion_mult(&temp, &temp, &conjugate);
	vec3_init(result, temp.i, temp.j, temp.k);
}

void quaternion_from_matrix(t_quaternion *result, const t_matrix3x3 *matrix)
{
	float tr = matrix->xx + matrix->yy + matrix->zz;
	if (tr > 0) {
		float S = sqrt(tr+1) * 2;
		result->r = 0.25 * S;
		result->i = (matrix->yz - matrix->zy) / S;
		result->j = (matrix->zx - matrix->xz) / S;
		result->k = (matrix->xy - matrix->yx) / S;
	} else if ((matrix->xx > matrix->yy) && (matrix->xx > matrix->zz)) {
		float S = sqrtf(1 + matrix->xx - matrix->yy - matrix->zz) * 2;
		result->r = (matrix->yz - matrix->zy) / S;
		result->i = 0.25 * S;
		result->j = (matrix->yx + matrix->xy) / S;
		result->k = (matrix->zx + matrix->xz) / S;
	} else if (matrix->yy > matrix->zz) {
		float S = sqrtf(1 + matrix->yy - matrix->xx - matrix->zz) * 2;
		result->r = (matrix->zx - matrix->xz) / S;
		result->i = (matrix->yx + matrix->xy) / S;
		result->j = 0.25 * S;
		result->k = (matrix->zy + matrix->yz) / S;
	} else {
		float S = sqrtf(1 + matrix->zz - matrix->xx - matrix->yy) * 2;
		result->r = (matrix->xy - matrix->yx) / S;
		result->i = (matrix->zx + matrix->xz) / S;
		result->j = (matrix->zy + matrix->yz) / S;
		result->k = 0.25 * S;
	}

	/* Test code
	t_vec3 tempA;
	t_vec3 tempB;

	quaternion_mult_vec3(&tempA, result, vec3_forward());
	vec3_init(&tempB, matrix->zx, matrix->zy, matrix->zz);
	vec3_subtract(&tempA, &tempA, &tempB);
	if (vec3_magnitude_sqr(&tempA) > 0.01)
		goto err_detected;

	quaternion_mult_vec3(&tempA, result, vec3_up());
	vec3_init(&tempB, matrix->yx, matrix->yy, matrix->yz);
	vec3_subtract(&tempA, &tempA, &tempB);
	if (vec3_magnitude_sqr(&tempA) > 0.01)
		goto err_detected;

	quaternion_mult_vec3(&tempA, result, vec3_right());
	vec3_init(&tempB, matrix->xx, matrix->xy, matrix->xz);
	vec3_subtract(&tempA, &tempA, &tempB);
	if (vec3_magnitude_sqr(&tempA) > 0.01)
		goto err_detected;

	return;

	t_vec3 got_forward;
	t_vec3 got_up;
	t_vec3 got_side;

	err_detected:
	return;
	quaternion_mult_vec3(&got_forward, result, vec3_forward());
	quaternion_mult_vec3(&got_up, result, vec3_up());
	quaternion_mult_vec3(&got_side, result, vec3_right());

	fprintf(stderr, "Found quat from matrix err!\n");
	fprintf(stderr, "	Quaternion: %.2f %.2f %.2f %.2f\n", result->r, result->i, result->j, result->k);
	fprintf(stderr, "	Forward:\n");
	fprintf(stderr, "		Matrix: %.2f %.2f %.2f\n", matrix->zx, matrix->zy, matrix->zz);
	fprintf(stderr, "		Got: %.2f %.2f %.2f\n", got_forward.x, got_forward.y, got_forward.z);
	fprintf(stderr, "	Up:\n");
	fprintf(stderr, "		Matrix: %.2f %.2f %.2f\n", matrix->yx, matrix->yy, matrix->yz);
	fprintf(stderr, "		Got: %.2f %.2f %.2f\n", got_up.x, got_up.y, got_up.z);
	fprintf(stderr, "	Side:\n");
	fprintf(stderr, "		Matrix: %.2f %.2f %.2f\n", matrix->xx, matrix->xy, matrix->xz);
	fprintf(stderr, "		Got: %.2f %.2f %.2f\n", got_side.x, got_side.y, got_side.z);
	*/
}

/*
**	From the 2 vectors calculate the side axis
**	Now we have 3 vectors all at 90 degrees, AKA: a matrix
**	Now we can call quaternion_from_matrix to create a quaternion from it
*/

void quaternion_from_forward_up(t_quaternion *result, const t_vec3 *forward, const t_vec3 *up)
{
	t_vec3 side;
	t_vec3 a_up;
	vec3_cross(&side, forward, up);
	vec3_normalize(&side, &side);
	vec3_cross(&a_up, &side, forward);

	t_matrix3x3 matrix;
	matrix.xx = side.x;
	matrix.xy = side.y;
	matrix.xz = side.z;

	matrix.yx = a_up.x;
	matrix.yy = a_up.y;
	matrix.yz = a_up.z;

	matrix.zx = -forward->x;
	matrix.zy = -forward->y;
	matrix.zz = -forward->z;

	quaternion_from_matrix(result, &matrix);
}