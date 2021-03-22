/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_ray_intersects.h                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:17:52 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 14:59:37 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_RAY_INTERSECTS_H
# define MINI_RT_RAY_INTERSECTS_H

# include "mini_rt_object.h"

bool			ray_intersects_aabb(const t_ray *ray,
					const t_aabb *aabb, float max_dist);

bool			ray_intersects_sphere(const t_object *object,
					const t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_plane(const t_object *object,
					const t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_square(const t_object *object,
					const t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_cylinder(const t_object *object,
					const t_ray *ray, t_ray_hit *hit);
bool			ray_intersects_triangle(const t_object *object,
					const t_ray *ray, t_ray_hit *hit);

bool			ray_intersects_cube(const t_object *object,
					const t_ray *ray, t_ray_hit *hit);

#endif
