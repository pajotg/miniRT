#include "mini_rt.h"
#include "test_utils.h"
#include <math.h>
#include "libft.h"

#include <stdio.h>

bool same_vec(const t_vec3* a, const t_vec3* b)
{
	t_vec3 temp;

	vec3_subtract(&temp, a,b);
	float magsqr = vec3_magnitude_sqr(&temp);
	return (magsqr < 0.01f && magsqr > -0.01f);
}

bool same_quat(const t_quaternion* a, const t_quaternion* b)
{
	return ((a->r - b->r) * (a->r - b->r)
	+ (a->i - b->i) * (a->i - b->i)
	+ (a->j - b->j) * (a->j - b->j)
	+ (a->k - b->k) * (a->k - b->k)) < 0.01f;
}

int main(int argc, char *argv[])
{
	t_vec3 temp;
	const t_quaternion* identity = quaternion_identity();

	TEST_START
		t_vec3 result;
		vec3_init(&temp, 1,1,1);
		quaternion_mult_vec3(&result, identity, &temp);
		if (!same_vec(&result, &temp))
			tu_ko_message_exit("Transforming a vec of 1,1,1 by the identity quaternion did not produce 1,1,1! it made: %.2f %.2f %.2f", result.x, result.y, result.z);
	TEST
		t_quaternion random;
		quaternion_init(&random, ft_randf() - 0.5f, ft_randf() - 0.5f, ft_randf() - 0.5f, ft_randf() - 0.5f);
		quaternion_normalize(&random, &random);

		t_quaternion resulta;
		t_quaternion resultb;
		quaternion_mult(&resulta, identity, &random);
		quaternion_mult(&resultb, &random, identity);

		if (!same_quat(&random, &resulta) && same_quat(&random,&resultb))
			tu_ko_message_exit("Multiplying by the identity quaternion did not produce the input! Input: (%.2f,%.2f,%.2f,%.2f), I*q = (%.2f,%.2f,%.2f,%.2f), q*I = (%.2f,%.2f,%.2f,%.2f)", random.r, random.i, random.j, random.k, resulta.r, resulta.i, resulta.j, resulta.k, resultb.r, resultb.i, resultb.j, resultb.k);
	TEST
		t_quaternion random;
		quaternion_init(&random, ft_randf() - 0.5f, ft_randf() - 0.5f, ft_randf() - 0.5f, ft_randf() - 0.5f);
		quaternion_normalize(&random, &random);

		t_quaternion resulta;
		t_quaternion resultb;
		quaternion_mult(&resulta, identity, &random);
		quaternion_conjugate(&random, &random);
		quaternion_mult(&resultb, &random, &resulta);

		if (!same_quat(&resultb, identity))
			tu_ko_message_exit("Multiplying by the identity by a random quaternion and then the conjugation did not produce the identity! Random: (%.2f,%.2f,%.2f,%.2f), I*q = (%.2f,%.2f,%.2f,%.2f), q'*(I*q = (%.2f,%.2f,%.2f,%.2f)", random.r, random.i, random.j, random.k, resulta.r, resulta.i, resulta.j, resulta.k, resultb.r, resultb.i, resultb.j, resultb.k);
	TEST
		t_vec3 x;
		t_vec3 y;
		t_vec3 z;
		quaternion_mult_vec3(&x, identity, vec3_right());
		quaternion_mult_vec3(&y, identity, vec3_up());
		quaternion_mult_vec3(&z, identity, vec3_forward());

		if (!same_vec(&x, vec3_right()))
			tu_ko_message_exit("I * (1,0,0) != (1,0,0) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(&y, vec3_up()))
			tu_ko_message_exit("I * (0,1,0) != (0,1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(&z, vec3_forward()))
			tu_ko_message_exit("I * (0,0,1) != (0,0,1) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_quaternion rotation;
		quaternion_from_AxisAngle(&rotation, vec3_up(), -M_PI/2);

		t_vec3 x;
		t_vec3 y;
		t_vec3 z;
		quaternion_mult_vec3(&x, &rotation, vec3_right());
		quaternion_mult_vec3(&y, &rotation, vec3_up());
		quaternion_mult_vec3(&z, &rotation, vec3_forward());

		if (!same_vec(&x, vec3_back()))
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (1,0,0) != (0,0,1) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(&y, vec3_up()))
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (0,1,0) != (0,1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(&z, vec3_right()))
			tu_ko_message_exit("AxisAngle(0,1,0,90) * (0,0,1) != (-1,0,0) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_quaternion rotation;
		quaternion_from_forward_up(&rotation, vec3_right(), vec3_down());

		t_vec3 x;
		t_vec3 y;
		t_vec3 z;
		quaternion_mult_vec3(&x, &rotation, vec3_right());
		quaternion_mult_vec3(&y, &rotation, vec3_up());
		quaternion_mult_vec3(&z, &rotation, vec3_back());

		if (!same_vec(&x, vec3_forward()))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (1,0,0) != (0,0,-1) it made: %.2f %.2f %.2f", x.x, x.y, x.z);
		if (!same_vec(&y, vec3_down()))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (0,1,0) != (0,-1,0) it made: %.2f %.2f %.2f", y.x, y.y, y.z);
		if (!same_vec(&z, vec3_left()))
			tu_ko_message_exit("forward_up(1,0,0,0,-1,0) * (0,0,1) != (-1,0,0) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_quaternion rotation;
		quaternion_from_forward_up(&rotation, vec3_up(), vec3_back());

		t_vec3 z;
		quaternion_mult_vec3(&z, &rotation, vec3_forward());

		if (!same_vec(&z, vec3_up()))
			tu_ko_message_exit("forward_up(0,1,0,0,0,1) * (0,0,-1) != (0,1,0) it made: %.2f %.2f %.2f", z.x, z.y, z.z);
	TEST
		t_object obj;
		t_object_sphere object_data;
		t_ray ray;
		t_ray_hit hit;

		vec3_init(&obj.transform.position, 0,0,0);
		obj.transform.rotation = *quaternion_identity();
		obj.object_data = &object_data;

		object_data.radius = 1;

		vec3_init(&ray.origin, 0,0,-5);
		vec3_init(&ray.direction, 0,0,1);

		hit.distance = INFINITY;
		if (!ray_intersects_sphere(&obj, &ray, &hit))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not hit!");
		if (!same_vec(&hit.location, vec3_forward()))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude location 0,0,-1!");
		if (!same_vec(&hit.normal, vec3_forward()))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude normal 0,0,-1!");
		if (hit.distance != 4)
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude distance 4!");

		vec3_init(&ray.origin, 0.5,0,-5);
		hit.distance = INFINITY;
		if (!ray_intersects_sphere(&obj, &ray, &hit))
			tu_ko_message_exit("Ray to sphere at 0,0,0 with r=1 with ray origin = 0.5,0,-5 and dir = 0,0,1 did not hit!");
	TEST
		for (int i = 0; i < 100; i++)
		{
			t_vec3 vecA;
			vec3_init(&vecA,
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1
			);
			vec3_normalize(&vecA, &vecA);

			t_vec3 vecB;
			vec3_init(&vecB,
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1,
				ft_randf() * 2 - 1
			);
			vec3_normalize(&vecB, &vecB);

			t_quaternion rotation;
			quaternion_from_forward_up(&rotation, &vecA,&vecB);
			t_vec3 z;
			quaternion_mult_vec3(&z, &rotation, vec3_forward());
			if (!same_vec(&z, &vecA))
				tu_ko_message_exit("%i forward_up( %.2f,%.2f,%.2f | %.2f,%.2f,%.2f ) * (0,0,-1) != (%.2f,%.2f,%.2f) it made: %.2f %.2f %.2f", i,
				vecA.x, vecA.y, vecA.z,
				vecB.x, vecB.y, vecB.z,
				vecA.x, vecA.y, vecA.z,
				z.x, z.y, z.z
			);
		}
	TEST
		t_object obj;
		t_object_square object_data;
		t_ray ray;
		t_ray_hit hit;

		vec3_init(&obj.transform.position, 0,0,0);
		obj.transform.rotation = *quaternion_identity();
		obj.object_data = &object_data;

		object_data.size = 1;

		vec3_init(&ray.origin, 0,0,-5);
		vec3_init(&ray.direction, 0,0,1);

		hit.distance = INFINITY;
		if (!ray_intersects_square(&obj, &ray, &hit))
			tu_ko_message_exit("Ray to square at 0,0,0 with s=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not hit!");
		if (!same_vec(&hit.location, vec3_forward()))
			tu_ko_message_exit("Ray to square at 0,0,0 with s=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude location 0,0,-1!");
		if (!same_vec(&hit.normal, vec3_forward()))
			tu_ko_message_exit("Ray to square at 0,0,0 with s=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude normal 0,0,-1!");
		if (hit.distance != 4)
			tu_ko_message_exit("Ray to square at 0,0,0 with s=1 with ray origin = 0,0,-5 and dir = 0,0,1 did not procude distance 4!");

		vec3_init(&ray.origin, 0.5,0,-5);
		hit.distance = INFINITY;
		if (!ray_intersects_square(&obj, &ray, &hit))
			tu_ko_message_exit("Ray to square at 0,0,0 with s=1 with ray origin = 0.5,0,-5 and dir = 0,0,1 did not hit!");
	TEST_END
	return (0);
}