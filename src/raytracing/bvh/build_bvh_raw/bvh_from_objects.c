#include "stupid_norm.h"

//printf("	Merged object %p (%.2f) with %p (%.2f) into BVH: %p (%.2f)\n",
//new->a, aabb_volume(&((t_object*)new->a)->aabb), new->b,
//aabb_volume(&((t_object*)new->b)->aabb), new, aabb_volume(&new->aabb));
//printf("	Copying object %p (%.2f) into BVH: %p (%.2f)\n", new->a,
//aabb_volume(&((t_object*)new->a)->aabb), new, aabb_volume(&new->aabb));
// pop is fastest
// remove the other index
// Build bvh with only this object

static size_t	find_other_object(t_object *base, t_list *objects, t_list *
	valid_indexes)
{
	size_t		other;
	t_aabb		*other_aabb;
	float		min_score;
	float		score;
	size_t		target_index;

	min_score = INFINITY;
	target_index = valid_indexes->count;
	other = 0;
	while (other < valid_indexes->count)
	{
		other_aabb = &((t_object *)list_index(objects, *((size_t *)list_index(
							valid_indexes, other))))->aabb;
		score = get_score(&base->aabb, other_aabb);
		if (score <= min_score)
		{
			min_score = score;
			target_index = other;
		}
		other++;
	}
	return (target_index);
}

bool	bvh_from_objects(t_list *bvh_get, t_list *bvh_put, t_list *objects,
	t_list *valid_indexes)
{
	size_t		target_index;
	t_bvh		*new;
	t_object	*base;

	while (valid_indexes->count > 0)
	{
		base = list_index(objects, *((size_t *)list_pop(valid_indexes)));
		target_index = find_other_object(base, objects, valid_indexes);
		if (target_index >= valid_indexes->count)
			new = bvh_combine_objects(base, NULL);
		else
		{
			new = bvh_combine_objects(base, list_index(objects, *((size_t *)
							list_index(valid_indexes, target_index))));
			list_remove(valid_indexes, target_index);
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
