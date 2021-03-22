/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   build_bvh.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 15:57:59 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/22 14:43:25 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_bvh.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_object.h"

#include <stdio.h>	// bad

// TODO: handle malloc errors better

t_bvh	*bvh_build_from_indexes(t_list *objects, t_list *valid_indexes)
{
	t_list	valid_indexes_a;
	t_list	valid_indexes_b;
	t_bvh	*new;

	if (valid_indexes->count > 200)
	{
		list_init(&valid_indexes_a, sizeof(size_t));
		list_init(&valid_indexes_b, sizeof(size_t));
		if (!split_objects(objects, valid_indexes, &valid_indexes_a,
				&valid_indexes_b))
			return (NULL);
		new = bvh_combine_bvh(
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
