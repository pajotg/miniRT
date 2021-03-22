/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_material_data.h                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 14:34:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/05 13:47:10 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MATERIAL_DATA_H
# define MINI_RT_MATERIAL_DATA_H

# include "mini_rt_material.h"
# include "ft_shared_ptr.h"

typedef struct s_material_diffuse
{
	t_color_hdr	color;
}				t_material_diffuse;

typedef struct s_material_emissive
{
	t_color_hdr	color;
}				t_material_emissive;

typedef struct s_material_perfect_mirror
{
	t_color_hdr	color;
}				t_material_perfect_mirror;

typedef struct s_material_checkerboard
{
	float		i_checker_size;
	t_color_hdr	color;
	t_shared_pt	*mat;
}				t_material_checkerboard;

typedef struct s_material_glossy
{
	float		roughness;
	t_color_hdr	color;
}				t_material_glossy;

typedef struct s_material_transparant
{
	t_color_hdr	color;
}				t_material_transparant;

typedef struct s_material_mix
{
	float		ratio;
	t_shared_pt	*a;
	t_shared_pt	*b;
}				t_material_mix;

typedef struct s_material_additive
{
	t_shared_pt	*a;
	t_shared_pt	*b;
}				t_material_additive;

void			material_free(t_material *material);

bool			material_diffuse_init(t_material *material,
					const t_color_hdr *color);
bool			material_emissive_init(t_material *material,
					const t_color_hdr *color);
bool			material_perfect_mirror_init(t_material *material,
					const t_color_hdr *color);
bool			material_additive_init(t_material *material, t_shared_pt *a,
					t_shared_pt *b);
bool			material_mix_init(t_material *material, float ratio,
					t_shared_pt *a, t_shared_pt *b);
bool			material_checkerboard_init(t_material *material,
					float checker_size, const t_color_hdr *color,
					t_shared_pt *mat);

#endif