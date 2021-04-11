#include "stupid_norm.h"

// we must merge with this one, cant say: nope

float	get_score(t_aabb *a, t_aabb *b)
{
	float	new_volume;
	float	a_volume;
	float	b_volume;
	t_aabb	new;

	new = *a;
	aabb_extend_box(&new, b);
	new_volume = aabb_volume(&new);
	a_volume = aabb_volume(a);
	b_volume = aabb_volume(b);
	if (a_volume == INFINITY)
	{
		if (a_volume == INFINITY)
			return (-b_volume);
		return (-a_volume);
	}
	return (new_volume);
}

t_bvh	*bvh_combine_objects(t_object *a, t_object *b)
{
	if (b)
		return (bvh_combine_leaf(a, &a->aabb, b, &b->aabb));
	return (bvh_combine_leaf(a, &a->aabb, NULL, NULL));
}

void	bvh_free_simple(t_bvh *bvh)
{
	bvh_free(bvh, NULL);
}
