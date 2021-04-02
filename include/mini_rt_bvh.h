/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_bvh.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 15:54:05 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/29 15:33:43 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_BVH_H
# define MINI_RT_BVH_H

# include "ft_aabb.h"
# include "ft_list.h"

/*
**	if is_leaf is set, then a and b are object pointers
**	otherwise, a and b aree bvh pointers
*/

typedef void	(*t_leaf_free_func)(void* ptr);

typedef struct s_bvh
{
	bool	is_leaf;
	t_aabb	aabb;
	void	*a;
	void	*b;
}				t_bvh;

void	bvh_free(t_bvh *bvh, t_leaf_free_func free_leaf_nodes);
t_bvh	*bvh_combine_bvh(t_bvh *a, t_bvh *b);
t_bvh	*bvh_combine_leaf(void *a, t_aabb *a_aabb, void *b, t_aabb *b_aabb);

t_bvh	*bvh_build(t_list *objects);
t_bvh	*bvh_build_from_indexes(t_list *objects, t_list *valid_indexes);
t_bvh	*bvh_build_from_indexes_raw(t_list *objects, t_list *valid_indexes);

bool	split_objects(const t_list *base, const t_list *valid_indexes, t_list
			*valid_indexes_a, t_list *valid_indexes_b);

#endif