/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_objects.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 14:36:34 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 13:18:32 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_OBJECTS_H
# define MINI_RT_OBJECTS_H

# include "mini_rt_color_math_utils.h"
# include <stdbool.h>

/*
**	Forward declare the t_object struct
*/

struct s_object;

typedef bool	(*t_object_intersect_func)(struct s_object* object,
	t_ray* ray, t_ray_hit* hit);

typedef struct	s_object
{
	t_transform				transform;
	void					*object_data;
	t_object_intersect_func	intersect_func;
}				t_object;

typedef struct	s_object_sphere
{
	t_color_hdr	color;
	float		radius;
}				t_object_sphere;

typedef struct	s_object_plane
{
	t_color_hdr color;
}				t_object_plane;

typedef struct	s_object_square
{
	t_color_hdr	color;
	float		size;
}				t_object_square;

typedef struct	s_object_cylinder
{
	t_color_hdr	color;
	float		radius;
	float		height;
}				t_object_cylinder;

typedef struct	s_object_triangle
{
	t_color_hdr	color;
	t_vec3		second_point;
	t_vec3		third_point;
}				t_object_triangle;

typedef struct	s_object_cube
{
	t_color_hdr	color;
	float		size;
}				t_object_cube;

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
