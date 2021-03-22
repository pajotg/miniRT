/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_object_data.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:15:03 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 15:16:59 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_OBJECT_DATA_H
# define MINI_RT_OBJECT_DATA_H

# include "mini_rt_color.h"
# include "ft_vec3.h"

typedef struct s_object_sphere
{
	float		radius;
}				t_object_sphere;

typedef struct s_object_plane
{
}				t_object_plane;

typedef struct s_object_square
{
	float		extends;
}				t_object_square;

typedef struct s_object_cylinder
{
	float		radius;
	float		height_extends;
}				t_object_cylinder;

typedef struct s_object_triangle
{
	t_vec3		second_point;
	t_vec3		third_point;
}				t_object_triangle;

typedef struct s_object_cube
{
	float		extends;
}				t_object_cube;

#endif
