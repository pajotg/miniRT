/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_material.h                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 14:34:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 13:32:39 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MATERIAL_H
# define MINI_RT_MATERIAL_H

# include "mini_rt_raycast.h"

typedef struct s_material_parse_data
{
	int			depth;
	const t_ray	*in_ray;
	t_ray_hit	*hit;
}				t_material_trace_data;

struct	s_material;

typedef void	(*t_material_func)(const t_scene *scene,
	const void *material_data, const t_material_trace_data *trace_data,
	t_color_hdr *o_out);

typedef void	(*t_material_free_data_func)(void *data);

typedef struct s_material
{
	void						*material_data;
	t_material_func				material_func;
	t_material_free_data_func	material_free_data_func;
}				t_material;

#endif