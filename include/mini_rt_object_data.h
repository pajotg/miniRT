/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_object_data.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:15:03 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:00:52 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_OBJECT_DATA_H
# define MINI_RT_OBJECT_DATA_H

# include "mini_rt_color.h"
# include "ft_vec3.h"

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
	float		extends;
}				t_object_square;

typedef struct	s_object_cylinder
{
	t_color_hdr	color;
	float		radius;
	float		height_extends;
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
	float		extends;
}				t_object_cube;

#endif
