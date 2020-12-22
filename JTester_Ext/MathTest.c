#include "miniRT_math_utils.h"
#include "test_utils.h"

int main(int argc, char *argv[])
{
	TEST_START
		t_quaternion identity = quaternion_identity();

		t_vec3 result = quaternion_mult_vec3(identity, vec3_new(1,1,1));
		float magsqr = vec3_magnitude_sqr(vec3_subtract(result, vec3_new(1,1,1)));
		if (magsqr > 0.01f || magsqr < -0.01f)
			tu_ko_message_exit("Transforming a vec of 1,1,1 by the identity quaternion did not produce 1,1,1! it made: %.2f %.2f %.2f", result.x, result.y, result.z);
	TEST_END
	return (0);
}