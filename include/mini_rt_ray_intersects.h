/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_ray_intersects.h                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:17:52 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:58:13 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RAY_INTERSECTS_H
# define MINI_RT_RAY_INTERSECTS_H

# include "mini_rt_object.h"

bool			ray_intersects_aabb(t_ray *ray,
	t_aabb *aabb, float max_dist);

bool			ray_intersects_sphere(t_object *object,
	t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_plane(t_object *object,
	t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_square(t_object *object,
	t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_cylinder(t_object *object,
	t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_triangle(t_object *object,
	t_ray *ray, t_ray_hit *hit);

bool			ray_intersects_cube(t_object *object,
	t_ray *ray, t_ray_hit *hit);

#endif
