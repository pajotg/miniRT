#include "mini_rt.h"
#include "test_utils.h"
#include <math.h>
#include "libft.h"

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
			tu_ko_message_exit("I * (0,0,1) != (0,0,1) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
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
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (0,0,1) != (-1,0,0) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_quaternion rotation = quaternion_from_forward_up(vec3_new(1,0,0),vec3_new(0,-1,0));

		t_vec3 x = quaternion_mult_vec3(rotation, vec3_new(1,0,0));
		t_vec3 y = quaternion_mult_vec3(rotation, vec3_new(0,1,0));
		t_vec3 z = quaternion_mult_vec3(rotation, vec3_new(0,0,-1));	// Negative z = forward

		if (!same_vec(x, vec3_new(0,0,-1)))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (1,0,0) != (0,0,-1) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(y, vec3_new(0,-1,0)))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (0,1,0) != (0,-1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(z, vec3_new(1,0,0)))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (0,0,1) != (-1,0,0) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_quaternion rotation = quaternion_from_forward_up(vec3_new(0,1,0),vec3_new(0,0,1));

		t_vec3 z = quaternion_mult_vec3(rotation, vec3_new(0,0,-1));	// Negative z = forward

		if (!same_vec(z, vec3_new(0,1,0)))
			tu_ko_message_exit("forward_up(0,1,0,0,0,1) * (0,0,-1) != (0,1,0) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_object obj;
		t_object_sphere object_data;
		t_ray ray;
		t_ray_hit hit;

		obj.transform.position = vec3_new(0,0,0);
		obj.transform.rotation = quaternion_identity();
		obj.object_data = &object_data;

		object_data.radius = 1;

		ray.origin = vec3_new(0,0,-5);
		ray.direction = vec3_new(0,0,1);

		hit.distance = INFINITY;
		if (!ray_intersects_sphere(&obj, &ray, &hit))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not hit!");
		if (!same_vec(hit.location, vec3_new(0,0,-1)))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude location 0,0,-1!");
		if (!same_vec(hit.normal, vec3_new(0,0,-1)))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude normal 0,0,-1!");
		if (hit.distance != 4)
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude distance 4!");

		ray.origin = vec3_new(0.5,0,-5);
		hit.distance = INFINITY;
		if (!ray_intersects_sphere(&obj, &ray, &hit))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0.5,0,-5 and dir = 0,0,1 did not hit!");
	TEST
		for (int i = 0; i < 100; i++)
		{
			t_vec3 vec = vec3_normalize(vec3_new(
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1
			));

			t_vec3 vecB = vec3_normalize(vec3_new(
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1
			));

			t_quaternion rotation = quaternion_from_forward_up(vec,vecB);
			t_vec3 z = quaternion_mult_vec3(rotation, vec3_new(0,0,-1));	// Negative z = forward
			if (!same_vec(z, vec))
				tu_ko_message_exit("%i forward_up( %.2f,%.2f,%.2f | %.2f,%.2f,%.2f ) * (0,0,-1) != (%.2f,%.2f,%.2f) it made: %.2f %.2f %.2f", i,
				vec.x, vec.y, vec.z,
				vecB.x, vecB.y, vecB.z,
				vec.x, vec.y, vec.z,
				z.x, z.y, z.z
			);
		}
	TEST_END
	return (0);
}