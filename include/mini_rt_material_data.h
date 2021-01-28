/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_material_data.h                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 14:34:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/28 15:43:26 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MATERIAL_DATA_H
# define MINI_RT_MATERIAL_DATA_H

#include "mini_rt_material.h"

void	material_free(t_material* material);

t_material*	material_diffuse_new(const t_color_hdr* color);

typedef struct	s_material_diffuse
{
	t_color_hdr color;
}				t_material_diffuse;

typedef struct	s_material_glossy
{
	float roughness;
	t_color_hdr color;
}				t_material_glossy;

typedef struct	s_material_transparant
{
	t_color_hdr color;
}				t_material_transparant;

typedef struct	s_material_mix
{
	float ratio;
	void* material_a;
	void* material_b;
	t_material_func material_a_func;
	t_material_func material_b_func;
}				t_material_mix;

typedef struct	s_material_additive
{
	void* material_a;
	void* material_b;
	t_material_func material_a_func;
	t_material_func material_b_func;
}				t_material_additive;

#endif