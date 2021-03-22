/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_raycast.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:19:59 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/20 15:44:28 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RAYCAST_H
# define MINI_RT_RAYCAST_H

# include <stdbool.h>
# include "ft_vec3.h"
# include "mini_rt_scene.h"

/*
**	Including mini_rt_object for the ray_hit would cause a cycle in our includes
**	So lets just forward declare it, its a pointer anyway, so we can do that
**	If you want to use that variable you will have to include mini_rt_object
*/

struct	s_object;

typedef struct s_ray_hit
{
	t_vec3			location;
	t_vec3			normal;
	float			distance;
	struct s_object	*object;
}				t_ray_hit;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}					t_ray;

bool				cast_ray(const t_scene *scene, const t_ray *ray,
						t_ray_hit *o_hit);
bool				cast_ray_max(const t_scene *scene, const t_ray *ray,
						t_ray_hit *o_hit,
						float max_dist);

#endif
