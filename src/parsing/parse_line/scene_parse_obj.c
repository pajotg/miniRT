/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene_parse_obj.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 12:14:41 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/06 13:35:13 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_obj.h"
#include "mini_rt_scene.h"
#include "mini_rt_parse_utils.h"
#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"

#include <stdlib.h>

#include <stdio.h>	// bad

void	transform_dir(const t_transform* transform, t_vec3 *pos)
{
	quaternion_mult_vec3(pos, &transform->rotation, pos);
}

void	transform_pos(const t_transform* transform, t_vec3 *pos)
{
	quaternion_mult_vec3(pos, &transform->rotation, pos);
	vec3_add(pos, &transform->position, pos);
}

bool	scene_parse_obj(t_scene *scene, char *line, int *curr)
{
	skip_whitespace(line, curr);

	t_transform transform;
	if (!read_transform(line, curr, &transform))
	{
		set_error(ft_strjoin("obj position and normal incorrectly formatted: ", line), true);
		return (false);
	}

	skip_whitespace(line, curr);

	// A ton of code to extract and combine the paths
	//	extract path from .rt file
	int start = *curr;
	while (!ft_isspace(line[*curr]))
		(*curr)++;
	int end = *curr;
	char* path = ft_substr(line, start, end - start);

	// 	extract base dir from scene path
	char* stop = ft_strrchr(scene->scene_path, '/');
	if (!stop)
		stop = scene->scene_path;
	char* base = ft_substr(scene->scene_path, 0, stop - scene->scene_path);
	//	combine base dir and path from .rt file
	char* true_path = ft_strjoin_va(3, base, "/", path);
	//	malloc management
	free(path);
	free(base);
	if (!true_path)
	{
		set_error("Failed malloc!", false);
		return (false);
	}

	t_obj *obj = parse_obj(true_path);
	free(true_path);
	if (!obj)
		return (false);

	skip_whitespace(line, curr);
	t_shared_pt* material = read_material(line, curr);
	if (!material)
	{
		free_obj(obj);
		return (false);
	}

	for (size_t i = 0; i < obj->vertexes.count; i++)
		transform_pos(&transform, list_index(&obj->vertexes, i));
	for (size_t i = 0; i < obj->normals.count; i++)
		transform_dir(&transform, list_index(&obj->normals, i));

	for (size_t i = 0; i < obj->faces.count; i++)
	{
		t_face* face = list_index(&obj->faces, i);
		t_vec3* a = list_index(&obj->vertexes, face->a.vertex_index);
		t_vec3* b = list_index(&obj->vertexes, face->b.vertex_index);
		t_vec3* c = list_index(&obj->vertexes, face->c.vertex_index);
		if (!a || !b || !c)
		{
			if (!a)
				set_error(ft_strjoin_va(4, "Could not index vertex a: ", ft_itoa(face->a.vertex_index), " max: ", ft_itoa(obj->vertexes.count)), true);
			else if (!b)
				set_error(ft_strjoin_va(4, "Could not index vertex b: ", ft_itoa(face->b.vertex_index), " max: ", ft_itoa(obj->vertexes.count)), true);
			else if (!c)
				set_error(ft_strjoin_va(4, "Could not index vertex c: ", ft_itoa(face->c.vertex_index), " max: ", ft_itoa(obj->vertexes.count)), true);
			free_obj(obj);
			return (false);
		}

		t_object new_triangle_obj;
		t_object_triangle *triangle = malloc(sizeof(t_object_triangle));
		if (!triangle)
		{
			set_error("Malloc failed", false);
			free_obj(obj);
			return (false);
		}
		shared_pt_get(material);

		new_triangle_obj.object_data = triangle;
		new_triangle_obj.material = material;
		new_triangle_obj.intersect_func = (t_object_intersect_func)ray_intersects_triangle;
		new_triangle_obj.transform.rotation = *quaternion_identity();

		new_triangle_obj.transform.position = *a;
		triangle->second_point = *b;
		triangle->third_point = *c;

		aabb_init(&new_triangle_obj.aabb, a);
		aabb_extend_point(&new_triangle_obj.aabb, b);
		aabb_extend_point(&new_triangle_obj.aabb, c);

		if (!list_push(&scene->objects, &new_triangle_obj))
		{
			set_error("list push failed", false);
			free(triangle);
			free_obj(obj);
			return (false);
		}
	}
	free_obj(obj);
	shared_pt_release_and_free(material);
	(void)scene;
	return (true);
}
