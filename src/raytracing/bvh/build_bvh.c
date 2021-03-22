/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   build_bvh.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 15:57:59 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/22 15:08:17 by jsimonis      ########   odam.nl         */
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
		bvh_free(a);
		bvh_free(b);
		return (NULL);
	}
	new = bvh_combine_bvh(a, b);
	if (!new)
	{
		bvh_free(a);
		bvh_free(b);
		return (NULL);
	}
	return (new);
}

t_bvh	*bvh_build_from_indexes(t_list *objects, t_list *valid_indexes)
{
	t_list	valid_indexes_a;
	t_list	valid_indexes_b;
	t_bvh	*new;
	bool	success;

	if (valid_indexes->count > 200)
	{
		success = list_init(&valid_indexes_a, sizeof(size_t));
		success = list_init(&valid_indexes_b, sizeof(size_t)) && success;
		if (!success || !split_objects(objects, valid_indexes, &valid_indexes_a,
				&valid_indexes_b))
		{
			list_un_init(&valid_indexes_a, NULL);
			list_un_init(&valid_indexes_b, NULL);
			return (NULL);
		}
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
