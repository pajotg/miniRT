#include "stupid_norm.h"

static bool	init_lists(t_list **bvh_get, t_list **bvh_put, int valid_count)
{
	bool	init_success;

	init_success = true;
	*bvh_get = list_new(sizeof(t_bvh *));
	init_success = init_success && bvh_get;
	*bvh_put = list_new(sizeof(t_bvh *));
	init_success = init_success && bvh_put;
	init_success = init_success && list_set_capacity(*bvh_put, sizeof(t_bvh *)
			 * valid_count / 2 + 1);
	init_success = init_success && list_set_capacity(*bvh_get, sizeof(t_bvh *)
			 * valid_count / 4 + 1);
	if (!init_success)
	{
		list_free(*bvh_get, (t_free_values)bvh_free_simple);
		list_free(*bvh_put, (t_free_values)bvh_free_simple);
	}
	return (init_success);
}

//printf("		Merging BVH %p (%.2f) with %p (%.2f) into %p (%.2f)\n", base,
//aabb_volume(&((t_bvh*)new->a)->aabb), new->b,
//aabb_volume(&((t_bvh*)new->b)->aabb),new, aabb_volume(&new->aabb));
//printf("	removing!\n");
//printf("	Combining!\n");
//printf("		Copying BVH %p (%.2f) into next round (%.2f)\n", base,
//aabb_volume(&new->aabb), aabb_volume(&new->aabb));
//printf("	target index: %lu\n", target_index);
//printf("		Other: %lu %p score: %.2f\n", i, other, score);
//printf("		Indexing %lu/%lu\n", i, bvh_get->count);
//printf("	Merging with base: %p (%lu left)\n", base, bvh_get->count);
// Alright, now in this loop, bva_get contains a list of bvh's that need to be
//merged into bvh_put
//printf("Creating %lu BVH's of %lu BVH's!\n", (bvh_get->count - 1) / 2 + 1,
//bvh_get->count);
// Swap
// clear old data

static size_t	find_other_bvh(t_bvh *base, t_list *bvh_get)
{
	size_t	target_index;
	size_t	i;
	float	min_score;
	float	score;
	t_bvh	*other;

	min_score = INFINITY;
	target_index = bvh_get->count;
	i = 0;
	while (i < bvh_get->count)
	{
		other = *((t_bvh **)list_index(bvh_get, i));
		score = get_score(&base->aabb, &other->aabb);
		if (score <= min_score)
		{
			min_score = score;
			target_index = i;
		}
		i++;
	}
	return (target_index);
}

static bool	do_single_merge(t_list *bvh_get, t_list *bvh_put)
{
	t_bvh	*new;
	t_bvh	*base;
	size_t	target_index;

	while (bvh_get->count > 0)
	{
		base = *((t_bvh **)list_pop(bvh_get));
		target_index = find_other_bvh(base, bvh_get);
		if (target_index >= bvh_get->count)
			new = base;
		else
		{
			new = bvh_combine_bvh(base, *((t_bvh **)list_index(bvh_get,
							target_index)));
			list_remove(bvh_get, target_index);
		}
		if (!new)
		{
			list_free(bvh_get, (t_free_values)bvh_free_simple);
			list_free(bvh_put, (t_free_values)bvh_free_simple);
			return (false);
		}
		list_push(bvh_put, &new);
	}
	return (true);
}

static t_bvh	*merge_into_single_bvh(t_list *bvh_get, t_list *bvh_put)
{
	t_list	*tmp;

	while (bvh_put->count > 1)
	{
		tmp = bvh_get;
		bvh_get = bvh_put;
		bvh_put = tmp;
		list_clear(bvh_put, NULL);
		if (!do_single_merge(bvh_get, bvh_put))
			return (NULL);
	}
	return (*((t_bvh **)list_index(bvh_put, 0)));
}

// What a horrific function, i guess this is what happens when you code late
//t_bvh *ret = *((t_bvh**)list_index(bvh_put, 0));
//printf("	Done merging objects into BVH's! now we have %lu bvh's!\n",
//bvh_put->count);
// first iteration merges objects into BVH's
//printf("Creating BVH of %lu indexes!\n", valid_indexes->count);
//	list_push(&valid_indexes, &i);
//for (size_t i = 0; i < objects->count; i++)
// Populate the list

t_bvh	*bvh_build_from_indexes_raw(t_list *objects, t_list *valid_indexes)
{
	t_bvh	*ret;
	t_list	*bvh_get;
	t_list	*bvh_put;

	if (!init_lists(&bvh_get, &bvh_put, valid_indexes->count))
		return (NULL);
	if (!bvh_from_objects(bvh_get, bvh_put, objects, valid_indexes))
		return (NULL);
	ret = merge_into_single_bvh(bvh_get, bvh_put);
	if (!ret)
		return (NULL);
	list_free(bvh_get, NULL);
	list_free(bvh_put, NULL);
	return (ret);
}
