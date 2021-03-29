#include "mini_rt_bvh.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_object.h"

#include <stdio.h>	// bad

void	bvh_free(t_bvh *bvh, t_leaf_free_func free_leaf_nodes)
{
	if (bvh->is_leaf)
	{
		if (free_leaf_nodes)
		{
			free_leaf_nodes(bvh->a);
			free_leaf_nodes(bvh->b);
		}
	}
	else
	{
		bvh_free(bvh->a, free_leaf_nodes);
		bvh_free(bvh->b, free_leaf_nodes);
	}
	free(bvh);
}

t_bvh	*bvh_combine_bvh(t_bvh *a, t_bvh *b)
{
	t_bvh	*new;

	new = malloc(sizeof(t_bvh));
	if (!new)
		return (NULL);
	new->is_leaf = false;
	new->a = a;
	new->b = b;
	new->aabb = a->aabb;
	aabb_extend_box(&new->aabb, &b->aabb);
	return (new);
}

t_bvh	*bvh_combine_leaf(void *a, t_aabb *a_aabb, void *b, t_aabb *b_aabb)
{
	t_bvh	*new;

	new = malloc(sizeof(t_bvh));
	if (!new)
		return (NULL);
	new->is_leaf = true;
	new->a = a;
	new->b = b;
	new->aabb = *a_aabb;
	if (b)
		aabb_extend_box(&new->aabb, b_aabb);
	return (new);
}
