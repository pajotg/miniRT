/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 15:57:59 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/06 13:21:53 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_bvh.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_object.h"

void	bvh_free(t_bvh* bvh)
{
	if (!bvh->is_leaf)
	{
		bvh_free(bvh->a);
		bvh_free(bvh->b);
	}
	free(bvh);
}

#include <stdio.h>	// bad

t_bvh	*bvh_combine_bvh(t_bvh* a, t_bvh* b)
{
	t_bvh *new = malloc(sizeof(t_bvh));
	if (!new)
		return (NULL);
	new->is_leaf = false;
	new->a = a;
	new->b = b;
	new->aabb = a->aabb;
	aabb_extend_box(&new->aabb, &b->aabb);
	return new;
}

t_bvh	*bvh_combine_leaf(void *a, t_aabb* a_aabb, void *b, t_aabb* b_aabb)
{
	t_bvh *new = malloc(sizeof(t_bvh));
	if (!new)
		return (NULL);
	new->is_leaf = true;
	new->a = a;
	new->b = b;
	new->aabb = *a_aabb;
	if (b)
	{
		/*
		printf("		Expanding box! %.2f %.2f %.2f - %.2f %.2f %.2f with %.2f %.2f %.2f - %.2f %.2f %.2f\n",
			new->aabb.min.x,new->aabb.min.y,new->aabb.min.z,
			new->aabb.max.x,new->aabb.max.y,new->aabb.max.z,
			b_aabb->min.x,b_aabb->min.y,b_aabb->min.z,
			b_aabb->max.x,b_aabb->max.y,b_aabb->max.z
		);
		*/
		aabb_extend_box(&new->aabb, b_aabb);
	}
	return new;
}

static t_bvh	*bvh_combine_objects(t_object *a, t_object *b)
{
	if (b)
		return bvh_combine_leaf(a, &a->aabb, b, &b->aabb);
	return bvh_combine_leaf(a, &a->aabb, NULL, NULL);
}

static float get_score(t_aabb *a, t_aabb *b)
{
	t_aabb new;
	new = *a;
	aabb_extend_box(&new, b);

	float new_volume = aabb_volume(&new);
	float a_volume = aabb_volume(a);
	float b_volume = aabb_volume(b);

	if (a_volume == INFINITY)	// we must merge with this one, cant say: nope
	{
		if (a_volume == INFINITY)
			return (-b_volume);
		return (-a_volume);
	}

	return new_volume;
}

// What a horrific function, i guess this is what happens when you code late
t_bvh	*bvh_build(t_list *objects)
{
	t_list valid_indexes;
	t_list *bvh_get;
	t_list *bvh_put;

	bool init_success = true;
	init_success = list_init(&valid_indexes, sizeof(size_t)) && init_success;
	bvh_get = list_new(sizeof(t_bvh*)); init_success = init_success && bvh_get;
	bvh_put = list_new(sizeof(t_bvh*)); init_success = init_success && bvh_put;
	init_success = init_success && list_set_capacity(bvh_put, sizeof(t_bvh*) * objects->count / 2 + 1);
	init_success = init_success && list_set_capacity(bvh_get, sizeof(t_bvh*) * objects->count / 4 + 1);
	if (!init_success)
	{
		list_un_init(&valid_indexes, NULL);
		list_free(bvh_get, (t_free_values)bvh_free);
		list_free(bvh_put, (t_free_values)bvh_free);
		return (NULL);
	}
	// Populate the list
	for (size_t i = 0; i < objects->count; i++)
		list_push(&valid_indexes, &i);

	//printf("Creating BVH of %lu objects!\n", objects->count);
	// first iteration merges objects into BVH's
	while (valid_indexes.count > 0)
	{
		float min_score = INFINITY;
		size_t target_index = valid_indexes.count;
		t_object* base = list_index(objects, *((size_t*)list_pop(&valid_indexes)));	// pop is fastest
		for (size_t other = 0; other < valid_indexes.count; other++)
		{
			size_t other_index = *((size_t*)list_index(&valid_indexes, other));
			t_aabb *other_aabb = &((t_object*)list_index(objects, other_index))->aabb;

			float score = get_score(&base->aabb, other_aabb);
			if (score <= min_score)
			{
				min_score = score;
				target_index = other;
			}
		}
		t_bvh *new;
		if (target_index >= valid_indexes.count)	// Build bvh with only this object
		{
			new = bvh_combine_objects(base, NULL);
			//printf("	Copying object %p (%.2f) into BVH: %p (%.2f)\n", new->a, volume(&((t_object*)new->a)->aabb), new, volume(&new->aabb));
		}
		else
		{
			size_t other_index = *((size_t*)list_index(&valid_indexes, target_index));
			new = bvh_combine_objects(base, list_index(objects, other_index));
			list_remove(&valid_indexes, target_index);	// remove the other index
			//printf("	Merged object %p (%.2f) with %p (%.2f) into BVH: %p (%.2f)\n", new->a, volume(&((t_object*)new->a)->aabb), new->b, volume(&((t_object*)new->b)->aabb), new, volume(&new->aabb));
		}
		if (!new)
		{
			list_un_init(&valid_indexes, NULL);
			list_free(bvh_get, (t_free_values)bvh_free);
			list_free(bvh_put, (t_free_values)bvh_free);
			return (NULL);
		}
		list_push(bvh_put, &new);
	}
	list_un_init(&valid_indexes, NULL);	// dont need this anymore
	//printf("	Done merging objects into BVH's! now we have %lu bvh's!\n", bvh_put->count);

	while (bvh_put->count > 1)
	{
		// Swap
		t_list *tmp;
		tmp = bvh_get;
		bvh_get = bvh_put;
		bvh_put = tmp;
		list_clear(bvh_put, NULL);	// clear old data

		//printf("Creating %lu BVH's of %lu BVH's!\n", (bvh_get->count - 1) / 2 + 1, bvh_get->count);
		// Alright, now in this loop, bva_get contains a list of bvh's that need to be merged into bvh_put
		while (bvh_get->count > 0)
		{
			float min_score = INFINITY;
			size_t target_index = bvh_get->count;

			t_bvh* base = *((t_bvh**)list_pop(bvh_get));
			//printf("	Merging with base: %p (%lu left)\n", base, bvh_get->count);
			for (size_t i = 0; i < bvh_get->count; i++)
			{
				//printf("		Indexing %lu/%lu\n", i, bvh_get->count);
				t_bvh* other = *((t_bvh**)list_index(bvh_get, i));
				float score = get_score(&base->aabb, &other->aabb);
				//printf("		Other: %lu %p score: %.2f\n", i, other, score);
				if (score <= min_score)
				{
					min_score = score;
					target_index = i;
				}
			}
			//printf("	target index: %lu\n", target_index);

			t_bvh *new;
			if (target_index >= bvh_get->count)
			{
				new = base;
				//printf("		Copying BVH %p (%.2f) into next round (%.2f)\n", base, volume(&new->aabb), volume(&new->aabb));
			}
			else
			{
				//printf("	Combining!\n");
				new = bvh_combine_bvh(base, *((t_bvh**)list_index(bvh_get, target_index)));
				//printf("	removing!\n");
				list_remove(bvh_get, target_index);
				//printf("		Merging BVH %p (%.2f) with %p (%.2f) into %p (%.2f)\n", base, volume(&((t_bvh*)new->a)->aabb), new->b, volume(&((t_bvh*)new->b)->aabb),new, volume(&new->aabb));
			}
			if (!new)
			{
				list_free(bvh_get, (t_free_values)bvh_free);
				list_free(bvh_put, (t_free_values)bvh_free);
				return (NULL);
			}
			list_push(bvh_put, &new);
		}
	}
	t_bvh *ret = *((t_bvh**)list_index(bvh_put, 0));
	list_free(bvh_get, NULL);
	list_free(bvh_put, NULL);
	return ret;
}