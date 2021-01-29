/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_material.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/29 15:08:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/29 16:52:44 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"
#include "libft.h"
#include "ft_error.h"
#include <stdlib.h>

// TODO:
//	Effects: normal disruption
//	Color disruption, checkerboard and rainbow effect
//	Materials: Specular, Glossy and transparent

/*
**	Pass in a material that has been created with init
**	If it fails it will also free the material data
*/

static t_shared_pt8* create_shared_ptr_from_material(t_material *material)
{
	t_material* material_ptr = malloc(sizeof(t_material));
	if (!material_ptr)
	{
		material->material_free_data_func(material->material_data);
		return (NULL);
	}
	*material_ptr = *material;
	t_shared_pt8* ptr = shared_pt8_new(material_ptr, (t_free_func)material_free);
	if (!ptr)
		material_free(material_ptr);
	return (ptr);
}

static t_shared_pt8 *read_diffuse(const char* str, int* current)
{
	t_color_hdr color;
	if (!read_color(str, current, false, &color))
	{
		set_error("Failed to read color of diffuse material!", false);
		return (NULL);
	}
	t_material material;
	if (!material_diffuse_init(&material, &color))
	{
		set_error("Failed to create diffuse material!", false);
		return (NULL);
	}
	return (create_shared_ptr_from_material(&material));
}

static t_shared_pt8 *read_mix(const char* str, int* current)
{
	float ratio;
	if (!read_float(str, current, &ratio))
	{
		set_error("Failed to read ratio of mix material!", false);
		return (NULL);
	}
	skip_whitespace(str, current);
	t_shared_pt8 *a = read_material(str, current);
	if (!a)
	{
		set_error(ft_strjoin("Failed to read first material of mix material!\nReason: ", get_last_error()), true);
		return (NULL);
	}
	skip_whitespace(str, current);
	t_shared_pt8 *b = read_material(str, current);
	if (!b)
	{
		shared_pt8_release_and_free(a);
		set_error(ft_strjoin("Failed to read second material of mix material!\nReason: ", get_last_error()), true);
		return (NULL);
	}

	t_material material;
	if (!material_mix_init(&material, ratio, a, b))
	{
		shared_pt8_release_and_free(a);
		shared_pt8_release_and_free(b);
		set_error("Failed to create mix material!", false);
		return (NULL);
	}
	// mix material now has a hold onto a&b
	shared_pt8_release_and_free(a);
	shared_pt8_release_and_free(b);
	return (create_shared_ptr_from_material(&material));
}

static t_shared_pt8 *read_additive(const char* str, int* current)
{
	t_shared_pt8 *a = read_material(str, current);
	if (!a)
	{
		set_error(ft_strjoin("Failed to read first material of additive material!\nReason: ", get_last_error()), true);
		return (NULL);
	}
	skip_whitespace(str, current);
	t_shared_pt8 *b = read_material(str, current);
	if (!b)
	{
		shared_pt8_release_and_free(a);
		set_error(ft_strjoin("Failed to read second material of additive material!\nReason: ", get_last_error()), true);
		return (NULL);
	}
	t_material material;
	if (!material_additive_init(&material, a, b))
	{
		shared_pt8_release_and_free(a);
		shared_pt8_release_and_free(b);
		set_error("Failed to create additive material!", false);
		return (NULL);
	}
	// Additive material now has a hold onto a&b
	shared_pt8_release_and_free(a);
	shared_pt8_release_and_free(b);
	return (create_shared_ptr_from_material(&material));
}

static t_shared_pt8 *read_checkerboard(const char* str, int* current)
{
	float checker_size = 0;
	if (!read_float(str, current, &checker_size))
	{
		set_error("Failed to read checker size of checkerboard material!", false);
		return (NULL);
	}
	skip_whitespace(str, current);
	t_color_hdr color;
	if (!read_color(str, current, false, &color))
	{
		set_error("Failed to read color of checkerboard material!", false);
		return (NULL);
	}
	skip_whitespace(str, current);
	t_shared_pt8 *base_material = read_material(str, current);
	if (!base_material)
	{
		set_error(ft_strjoin("Failed to read base material of checkerboard material!\nReason: ", get_last_error()), true);
		return (NULL);
	}
	t_material material;
	if (!material_checkerboard_init(&material, checker_size, &color, base_material))
	{
		shared_pt8_release_and_free(base_material);
		set_error("Failed to create checkerboard material!", false);
		return (NULL);
	}
	// checkerboard material now has a hold onto a&b
	shared_pt8_release_and_free(base_material);
	return (create_shared_ptr_from_material(&material));
}

/*
** A material is either
** a plain color, aka: diffuse only
** Defined by a tree like structure:
** M value [material] [material]	// mix
** A [material] [material]	// add
** D color	// diffuse
** S color roughness	// specular
** G color roughness	// glossy
** T color	// transparent
** examples of valid materials:
**	255,255,255
**	G 255,255,255
**	M 0.5 D 255,255,255 S 255,0,0
*/

t_shared_pt8 *read_material(const char* str, int* current)
{
	// Try default material
	t_color_hdr color;
	t_material material;
	if (read_color(str, current, false, &color))
	{
		if (!material_diffuse_init(&material, &color))
		{
			set_error("Failed to create diffuse material!", false);
			return (NULL);
		}
		return (create_shared_ptr_from_material(&material));
	}

	// Get what type of material it is
	t_shared_pt8 *(*read_ptr)(const char* str, int* current);

	if (str[*current] == 'D')
		read_ptr = read_diffuse;
	else if (str[*current] == 'M')
		read_ptr = read_mix;
	else if (str[*current] == 'A')
		read_ptr = read_additive;
	else if (str[*current] == 'C')
		read_ptr = read_checkerboard;
	else {
		set_error("Failed to detect type of material!", false);
		return (NULL);
	}

	// Read the material
	*current+=1;
	skip_whitespace(str, current);
	return (read_ptr(str, current));
}