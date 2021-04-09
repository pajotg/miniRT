/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_material_1.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/29 15:08:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/09 18:35:52 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

t_shared_pt	*read_specular(const char *str, int *current)
{
	float		pow;
	t_material	material;
	t_color_hdr	color;

	if (!read_color(str, current, false, &color))
	{
		set_error("Failed to read color of specular material!", false);
		return (NULL);
	}
	skip_char(str, current, ' ');
	if (!read_float(str, current, &pow))
	{
		set_error("Failed to read pow of specular material!", false);
		return (NULL);
	}
	if (!material_specular_init(&material, &color, pow))
	{
		set_error("Failed to create specular material!", false);
		return (NULL);
	}
	return (create_shared_ptr_from_material(&material));
}

static bool	read_additive_parse_str(const char *str, int *current, t_shared_pt *
	*a, t_shared_pt **b)
{
	*a = read_material(str, current);
	if (!*a)
	{
		set_error(ft_strjoin("Failed to read first material of additive materia"
				"l!\nReason: ", get_last_error()), true);
		return (false);
	}
	skip_whitespace(str, current);
	*b = read_material(str, current);
	if (!*b)
	{
		shared_pt_release_and_free(*a);
		set_error(ft_strjoin("Failed to read second material of additive materi"
				"al!\nReason: ", get_last_error()), true);
		return (false);
	}
	return (true);
}

// Additive material now has a hold onto a&b

t_shared_pt	*read_additive(const char *str, int *current)
{
	t_material	material;
	t_shared_pt	*a;
	t_shared_pt	*b;

	if (!read_additive_parse_str(str, current, &a, &b))
		return (NULL);
	if (!material_additive_init(&material, a, b))
	{
		shared_pt_release_and_free(a);
		shared_pt_release_and_free(b);
		set_error("Failed to create additive material!", false);
		return (NULL);
	}
	shared_pt_release_and_free(a);
	shared_pt_release_and_free(b);
	return (create_shared_ptr_from_material(&material));
}

// checkerboard material now has a hold onto a&b

static bool	read_checker_data(const char *str, int *current, float *
	checker_size, t_color_hdr *color)
{
	if (!read_float(str, current, checker_size))
	{
		set_error("Failed to read checker size of checkerboard material!",
			false);
		return (false);
	}
	skip_whitespace(str, current);
	if (!read_color(str, current, false, color))
	{
		set_error("Failed to read color of checkerboard material!", false);
		return (false);
	}
	return (true);
}

t_shared_pt	*read_checkerboard(const char *str, int *current)
{
	float		checker_size;
	t_color_hdr	color;
	t_material	material;
	t_shared_pt	*base_material;

	if (!read_checker_data(str, current, &checker_size, &color))
		return (NULL);
	skip_whitespace(str, current);
	base_material = read_material(str, current);
	if (!base_material)
	{
		set_error(ft_strjoin("Failed to read base material of checkerboard mate"
				"rial!\nReason: ", get_last_error()), true);
		return (NULL);
	}
	if (!material_checkerboard_init(&material, checker_size, &color,
			base_material))
	{
		shared_pt_release_and_free(base_material);
		set_error("Failed to create checkerboard material!", false);
		return (NULL);
	}
	shared_pt_release_and_free(base_material);
	return (create_shared_ptr_from_material(&material));
}
