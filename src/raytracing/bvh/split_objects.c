#include "mini_rt_bvh.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_object.h"

#include <stdio.h>	// bad

static t_aabb	get_total_aabb(const t_list *base, const t_list *valid_indexes)
{
	size_t	i;
	size_t	*index;
	t_aabb	total_aabb;

	index = list_index_unchecked(valid_indexes, 0);
	total_aabb = ((t_object*)list_index(base, *index))->aabb;
	i = 1;
	while (i < valid_indexes->count)
	{
		index = list_index_unchecked(valid_indexes, i);
		aabb_extend_box(&total_aabb, &((t_object*)list_index_unchecked(base,
					*index))->aabb);
		i++;
	}
	return (total_aabb);
}

static void	get_center_and_biggest_dir(t_aabb aabb, t_vec3 *center, t_vec3 *dir)
{
	int		i;
	float	biggest_size;
	float	size;
	t_vec3	center;
	int		biggest_axis;

	biggest_axis = -1;
	biggest_size = -1;
	i = 0;
	while (i < 3)
	{
		size = ((float *)&aabb.max)[i] - ((float *)&aabb.min)[i];
		if (size > biggest_size)
		{
			biggest_axis = i;
			biggest_size = size;
		}
		i++;
	}
	vec3_add(&center, &aabb.max, &aabb.min);
	vec3_scale(&center, &center, 0.5);
	*dir = (t_vec3){0, 0, 0};
	((float *)dir)[biggest_axis] = 1;
}

// TODO: Check for malloc errors

bool	split_objects(const t_list *base, const t_list *valid_indexes, t_list *
	valid_indexes_a, t_list *valid_indexes_b)
{
	size_t	i;
	t_vec3	current;
	t_aabb	*curr_aabb;
	t_vec3	center;
	t_vec3	dir;

	get_center_and_biggest_dir(get_total_aabb(base, valid_indexes), &center,
		&dir);
	i = 0;
	while (i < valid_indexes->count)
	{
		curr_aabb = &((t_object*)list_index(base, *(size_t*)
					list_index_unchecked(valid_indexes, i)))->aabb;
		vec3_add(&current, &curr_aabb->max, &curr_aabb->min);
		vec3_scale(&current, &current, 0.5);
		vec3_subtract(&current, &current, &center);
		if (vec3_dot(&current, &dir) > 0)
			list_push(valid_indexes_a, list_index_unchecked(valid_indexes, i));
		else
			list_push(valid_indexes_b, list_index_unchecked(valid_indexes, i));
		i++;
	}
	return (true);
}
