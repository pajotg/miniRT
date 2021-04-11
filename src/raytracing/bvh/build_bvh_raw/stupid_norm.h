#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_bvh.h"
# include <stdlib.h>
# include <math.h>
# include "mini_rt_object.h"

# include <stdio.h>	// bad

float	get_score(t_aabb *a, t_aabb *b);
t_bvh	*bvh_combine_objects(t_object *a, t_object *b);
void	bvh_free_simple(t_bvh *bvh);

#endif