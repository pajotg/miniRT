/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_objects.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 14:36:34 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 14:39:17 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_OBJECTS_H
# define MINI_RT_OBJECTS_H

#include "mini_rt_math_utils.h"

typedef t_ray_hit	(*t_object_intersect_func)(void* ObjectData, t_ray ray);

typedef struct	s_object
{
	t_transform Transform;
	void* ObjectData;
	t_object_intersect_func* IntersectFunc;
}				t_object;

typedef struct	s_object_sphere
{
	t_color_hdr color;
	float diameter;
}				t_object_sphere;

typedef struct	s_object_plane
{
	t_color_hdr color;
}				t_object_plane;

#endif