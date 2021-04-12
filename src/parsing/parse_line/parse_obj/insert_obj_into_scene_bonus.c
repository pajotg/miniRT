#include "ft_error.h"
#include "libft.h"
//#include "ft_parse_utils.h"
#include "mini_rt_parse_obj.h"
//#include "mini_rt_scene.h"
//#include "mini_rt_parse_utils.h"
#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"

#include <stdlib.h>

//#include <stdio.h>	// bad

//static void	transform_dir(const t_transform* transform, t_vec3 *pos)
//{
//	quaternion_mult_vec3(pos, &transform->rotation, pos);
//}

static void	transform_pos(const t_transform *transform, t_vec3 *pos)
{
	quaternion_mult_vec3(pos, &transform->rotation, pos);
	vec3_add(pos, &transform->position, pos);
}

static bool	get_vertex_positions(const t_obj *obj, size_t face_index, t_vec3 **
	arr)
{
	t_face	*face;

	face = list_index(&obj->faces, face_index);
	arr[0] = list_index(&obj->vertexes, face->a.vertex_index);
	arr[1] = list_index(&obj->vertexes, face->b.vertex_index);
	arr[2] = list_index(&obj->vertexes, face->c.vertex_index);
	if (!arr[0] || !arr[1] || !arr[2])
	{
		if (!arr[0])
			set_error(ft_strjoin_va(4, "Could not index vertex a: ", ft_itoa(
						face->a.vertex_index), " max: ", ft_itoa(obj
						->vertexes.count)), true);
		else if (!arr[1])
			set_error(ft_strjoin_va(4, "Could not index vertex b: ", ft_itoa(
						face->b.vertex_index), " max: ", ft_itoa(obj
						->vertexes.count)), true);
		else if (!arr[2])
			set_error(ft_strjoin_va(4, "Could not index vertex c: ", ft_itoa(
						face->c.vertex_index), " max: ", ft_itoa(obj
						->vertexes.count)), true);
		return (false);
	}
	return (true);
}

static bool	convert_to_triangle(t_object *new_triangle_obj, const t_obj *obj,
	size_t face_index, t_shared_pt *material)
{
	t_object_triangle	*triangle;
	t_vec3				*vertexes[3];

	get_vertex_positions(obj, face_index, vertexes);
	triangle = malloc(sizeof(t_object_triangle));
	if (!triangle)
	{
		set_error("Malloc failed", false);
		return (false);
	}
	shared_pt_get(material);
	new_triangle_obj->object_data = triangle;
	new_triangle_obj->material = material;
	new_triangle_obj->intersect_func = (t_object_intersect_func)
		ray_intersects_triangle;
	new_triangle_obj->transform.rotation = *quaternion_identity();
	new_triangle_obj->transform.position = *vertexes[0];
	triangle->second_point = *vertexes[1];
	triangle->third_point = *vertexes[2];
	aabb_init(&new_triangle_obj->aabb, vertexes[0]);
	aabb_extend_point(&new_triangle_obj->aabb, vertexes[1]);
	aabb_extend_point(&new_triangle_obj->aabb, vertexes[2]);
	return (true);
}

//	transform_dir(transform, list_index(&obj->normals, i));
//for (size_t i = 0; i < obj->normals.count; i++)

bool	insert_obj_into_scene(t_scene *scene, t_obj *obj,
	const t_transform *transform, t_shared_pt *material)
{
	size_t		i;
	t_object	new_triangle_obj;

	i = 0;
	while (i < obj->vertexes.count)
	{
		transform_pos(transform, list_index(&obj->vertexes, i));
		i++;
	}
	i = 0;
	while (i < obj->faces.count)
	{
		if (!convert_to_triangle(&new_triangle_obj, obj, i, material))
			return (NULL);
		if (!list_push(&scene->objects, &new_triangle_obj))
		{
			set_error("list push failed", false);
			free(new_triangle_obj.object_data);
			shared_pt_release_and_free(material);
			return (false);
		}
		i++;
	}
	return (true);
}
