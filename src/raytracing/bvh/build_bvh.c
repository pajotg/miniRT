/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   build_bvh.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 15:57:59 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/29 17:03:19 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_bvh.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_object.h"

#include <stdio.h>	// bad

static t_bvh	*try_combine_or_free(t_bvh *a, t_bvh *b)
{
	t_bvh	*new;

	if (!a || !b)
	{
		bvh_free(a, NULL);
		bvh_free(b, NULL);
		return (NULL);
	}
	new = bvh_combine_bvh(a, b);
	if (!new)
	{
		bvh_free(a, NULL);
		bvh_free(b, NULL);
		return (NULL);
	}
	return (new);
}

static bool	init_indexes_and_split(t_list *objects, t_list *valid_indexes,
	t_list	*valid_indexes_a, t_list	*valid_indexes_b)
{
	bool	success;

	success = list_init(valid_indexes_a, sizeof(size_t));
	success = list_init(valid_indexes_b, sizeof(size_t)) && success;
	if (!success || !split_objects(objects, valid_indexes, valid_indexes_a,
			valid_indexes_b))
	{
		list_un_init(valid_indexes_a, NULL);
		list_un_init(valid_indexes_b, NULL);
		return (false);
	}
	return (true);
}

t_bvh	*bvh_build_from_indexes(t_list *objects, t_list *valid_indexes)
{
	t_list	valid_indexes_a;
	t_list	valid_indexes_b;
	t_bvh	*new;

	if (valid_indexes->count > 200)
	{
		if (!init_indexes_and_split(objects, valid_indexes, &valid_indexes_a,
				&valid_indexes_b))
			return (NULL);
		if (valid_indexes_a.count == 0 || valid_indexes_b.count == 0)
			new = bvh_build_from_indexes_raw(objects, valid_indexes);
		else
			new = try_combine_or_free(
					bvh_build_from_indexes(objects, &valid_indexes_a),
					bvh_build_from_indexes(objects, &valid_indexes_b)
					);
		list_un_init(&valid_indexes_a, NULL);
		list_un_init(&valid_indexes_b, NULL);
		return (new);
	}
	return (bvh_build_from_indexes_raw(objects, valid_indexes));
}

//#include "ft_time.h"
//printf("BVH building took %.2fs! with %lu objects!\n", time_difference(&end, 
//&now), objects->count);
//t_time end = time_now();
//t_time now = time_now();
//printf("building BVH!\n");

t_bvh	*bvh_build(t_list *objects)
{
	size_t	i;
	t_bvh	*new;
	t_list	valid_indexes;

	list_init(&valid_indexes, sizeof(size_t));
	i = 0;
	while (i < objects->count)
	{
		list_push(&valid_indexes, &i);
		i++;
	}
	new = bvh_build_from_indexes(objects, &valid_indexes);
	list_un_init(&valid_indexes, NULL);
	return (new);
}
