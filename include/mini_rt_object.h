/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_object.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:15:03 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:55:47 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_OBJECT_H
# define MINI_RT_OBJECT_H

# include "mini_rt_raycast.h"
# include "ft_vec3.h"
# include "ft_quaternion.h"
# include "ft_aabb.h"

/*
**	Forward declare the t_object struct
*/

struct s_object;

typedef bool		(*t_object_intersect_func)(struct s_object* object,
	t_ray* ray, t_ray_hit* hit);

typedef struct		s_transform
{
	t_vec3			position;
	t_quaternion	rotation;
}					t_transform;

typedef struct		s_object
{
	t_transform				transform;
	t_aabb					aabb;
	void					*object_data;
	t_object_intersect_func	intersect_func;
}					t_object;

typedef struct		s_camera
{
	t_transform	transform;
	float		fov;
}					t_camera;

typedef struct		s_light
{
	t_vec3		position;
	t_color_hdr	color;
}					t_light;

typedef struct		s_directional_light
{
	t_vec3		direction;
	t_color_hdr	color;
}					t_directional_light;

#endif
