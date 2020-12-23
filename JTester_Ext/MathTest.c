#include "mini_rt_math_utils.h"
#include "test_utils.h"
#include <math.h>

#include <stdio.h>

bool same_vec(t_vec3 a, t_vec3 b)
{
	float magsqr = vec3_magnitude_sqr(vec3_subtract(a,b));
	return (magsqr < 0.01f && magsqr > -0.01f);
}

bool same_quat(t_quaternion a, t_quaternion b)
{
	return ((a.r - b.r) * (a.r - b.r)
	+ (a.i - b.i) * (a.i - b.i)
	+ (a.j - b.j) * (a.j - b.j)
	+ (a.k - b.k) * (a.k - b.k)) < 0.01f;

}

int main(int argc, char *argv[])
{
	TEST_START
		t_quaternion identity = quaternion_identity();

		t_vec3 result = quaternion_mult_vec3(identity, vec3_new(1,1,1));
		if (!same_vec(result, vec3_new(1,1,1)))
			tu_ko_message_exit("Transforming a vec of 1,1,1 by the identity quaternion did not produce 1,1,1! it made: %.2f %.2f %.2f", result.x, result.y, result.z);
	TEST
		t_quaternion identity = quaternion_identity();

		t_quaternion random = quaternion_normalize(quaternion_new(52,2,-2, -71));
		t_quaternion resulta = quaternion_mult(identity, random);
		t_quaternion resultb = quaternion_mult(random, identity);

		if (!same_quat(random, resulta) && same_quat(random,resultb))
			tu_ko_message_exit("Multiplying by the identity quaternion did not produce the input! Input: (%.2f,%.2f,%.2f,%.2f), I*q = (%.2f,%.2f,%.2f,%.2f), q*I = (%.2f,%.2f,%.2f,%.2f)", random.r, random.i, random.j, random.k, resulta.r, resulta.i, resulta.j, resulta.k, resultb.r, resultb.i, resultb.j, resultb.k);
	TEST
		t_quaternion identity = quaternion_identity();

		t_quaternion random = quaternion_normalize(quaternion_new(52,2,-2, -71));
		t_quaternion resulta = quaternion_mult(identity, random);
		t_quaternion resultb = quaternion_mult(quaternion_conjugate(random), resulta);

		if (!same_quat(resultb, identity))
			tu_ko_message_exit("Multiplying by the identity by a random quaternion and then the conjugation did not produce the identity! Random: (%.2f,%.2f,%.2f,%.2f), I*q = (%.2f,%.2f,%.2f,%.2f), q'*(I*q = (%.2f,%.2f,%.2f,%.2f)", random.r, random.i, random.j, random.k, resulta.r, resulta.i, resulta.j, resulta.k, resultb.r, resultb.i, resultb.j, resultb.k);
	TEST
		t_quaternion identity = quaternion_identity();

		t_vec3 x = quaternion_mult_vec3(identity, vec3_new(1,0,0));
		t_vec3 y = quaternion_mult_vec3(identity, vec3_new(0,1,0));
		t_vec3 z = quaternion_mult_vec3(identity, vec3_new(0,0,1));

		if (!same_vec(x, vec3_new(1,0,0)))
			tu_ko_message_exit("I * (1,0,0) != (1,0,0) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(y, vec3_new(0,1,0)))
			tu_ko_message_exit("I * (0,1,0) != (0,1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(z, vec3_new(0,0,1)))
			tu_ko_message_exit("I * (0,0,1) != (0,0,1) it made: %.2f %.2f %.2f", z.x, x.y, z.z);
	TEST
		t_quaternion rotation = quaternion_from_AxisAngle(vec3_new(0,1,0), -M_PI/2);

		t_vec3 x = quaternion_mult_vec3(rotation, vec3_new(1,0,0));
		t_vec3 y = quaternion_mult_vec3(rotation, vec3_new(0,1,0));
		t_vec3 z = quaternion_mult_vec3(rotation, vec3_new(0,0,1));

		if (!same_vec(x, vec3_new(0,0,1)))
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (1,0,0) != (0,0,1) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(y, vec3_new(0,1,0)))
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (0,1,0) != (0,1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(z, vec3_new(-1,0,0)))
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (0,0,1) != (-1,0,0) it made: %.2f %.2f %.2f", z.x, x.y, z.z);
	TEST
		t_quaternion rotation = quaternion_from_forward_up(vec3_new(1,0,0),vec3_new(0,-1,0));

		t_vec3 x = quaternion_mult_vec3(rotation, vec3_new(1,0,0));
		t_vec3 y = quaternion_mult_vec3(rotation, vec3_new(0,1,0));
		t_vec3 z = quaternion_mult_vec3(rotation, vec3_new(0,0,-1));	// Negative z = forward

		if (!same_vec(x, vec3_new(0,0,1)))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (1,0,0) != (0,0,1) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(y, vec3_new(0,-1,0)))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (0,1,0) != (0,-1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(z, vec3_new(-1,0,0)))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (0,0,1) != (-1,0,0) it made: %.2f %.2f %.2f", z.x, x.y, z.z);
	TEST_END
	return (0);
}