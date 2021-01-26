/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_raycast.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:19:59 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:00:44 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RAYCAST_H
# define MINI_RT_RAYCAST_H

# include <stdbool.h>
# include "ft_vec3.h"
# include "mini_rt_color.h"
# include "mini_rt_scene.h"

typedef struct		s_ray_hit
{
	t_vec3		location;
	t_vec3		normal;
	t_color_hdr	color;
	float		distance;
}					t_ray_hit;

typedef struct		s_ray
{
	t_vec3 origin;
	t_vec3 direction;
}					t_ray;

bool				cast_ray(t_scene *scene, t_ray *ray, t_ray_hit *o_hit);
bool				cast_ray_max(t_scene *scene, t_ray *ray, t_ray_hit *o_hit,
	float max_dist);

#endif
