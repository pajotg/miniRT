#include "stupid_norm.h"

t_shared_pt	*read_diffuse(const char *str, int *current)
{
	t_material	material;
	t_color_hdr	color;

	if (!read_color(str, current, false, &color))
	{
		set_error("Failed to read color of diffuse material!", false);
		return (NULL);
	}
	if (!material_diffuse_init(&material, &color))
	{
		set_error("Failed to create diffuse material!", false);
		return (NULL);
	}
	return (create_shared_ptr_from_material(&material));
}

t_shared_pt	*read_emissive(const char *str, int *current)
{
	t_material	material;
	t_color_hdr	color;

	if (!read_color(str, current, true, &color))
	{
		set_error("Failed to read color of emissive material!", false);
		return (NULL);
	}
	if (!material_emissive_init(&material, &color))
	{
		set_error("Failed to create emissive material!", false);
		return (NULL);
	}
	return (create_shared_ptr_from_material(&material));
}

t_shared_pt	*read_perfect_mirror(const char *str, int *current)
{
	t_material	material;
	t_color_hdr	color;

	if (!read_color(str, current, false, &color))
	{
		set_error("Failed to read color of perfect mirror material!", false);
		return (NULL);
	}
	if (!material_perfect_mirror_init(&material, &color))
	{
		set_error("Failed to create perfect mirror material!", false);
		return (NULL);
	}
	return (create_shared_ptr_from_material(&material));
}

static bool	read_mix_parse_str(const char *str, int *current, t_mix_data *md)
{
	if (!read_float(str, current, &md->ratio))
	{
		set_error("Failed to read ratio of mix material!", false);
		return (false);
	}
	skip_whitespace(str, current);
	md->a = read_material(str, current);
	if (!md->a)
	{
		set_error(ft_strjoin("Failed to read first material of mix material!\nR"
				"eason: ", get_last_error()), true);
		return (false);
	}
	skip_whitespace(str, current);
	md->b = read_material(str, current);
	if (!md->b)
	{
		shared_pt_release_and_free(md->a);
		set_error(ft_strjoin("Failed to read second material of mix material!"
				"\nReason: ", get_last_error()), true);
		return (false);
	}
	return (true);
}

// mix material now has a hold onto a&b

t_shared_pt	*read_mix(const char *str, int *current)
{
	t_material	material;
	t_mix_data	md;

	if (!read_mix_parse_str(str, current, &md))
		return (NULL);
	if (!material_mix_init(&material, md.ratio, md.a, md.b))
	{
		shared_pt_release_and_free(md.a);
		shared_pt_release_and_free(md.b);
		set_error("Failed to create mix material!", false);
		return (NULL);
	}
	shared_pt_release_and_free(md.a);
	shared_pt_release_and_free(md.b);
	return (create_shared_ptr_from_material(&material));
}
