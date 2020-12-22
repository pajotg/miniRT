/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_utils.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:19 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 19:47:50 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_UTILS_H
# define MINI_RT_PARSE_UTILS_H

#include "mini_rt_math_utils.h"

typedef t_ray_hit	(*t_object_intersect_func)(void* ObjectData, t_ray ray);

typedef struct	s_transform
{
	void* ObjectData;
	t_object_intersect_func* IntersectFunc;
}				t_transform;

typedef struct	s_object
{
	t_transform Transform;
	void* ObjectData;
	t_object_intersect_func* IntersectFunc;
}				t_object;

typedef struct	s_camera
{
	t_transform Transform;
}				t_camera;

typedef struct	s_scene
{
	int camera_count;
	t_camera* cameras;
	t_object* objects;
}				t_scene;

t_scene* parse_scene_file(int fd);

#endif