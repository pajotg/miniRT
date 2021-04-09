#include "stupid_norm.h"

static t_shared_pt	*read_default(t_color_hdr *
	color)
{
	t_material	material;

	if (!material_diffuse_init(&material, color))
	{
		set_error("Failed to create diffuse material!", false);
		return (NULL);
	}
	return (create_shared_ptr_from_material(&material));
}

// A material is either:
// 	a plain color, aka: diffuse only
// 	or Defined by a tree like structure:
//		M value [material] [material]	// mix
//		A [material] [material]	// add
//		D color	// diffuse
//		S color pow strength	// specular
//		G color roughness	// glossy
//		T color	// transparent
//	examples of valid materials:
//		255,255,255
//		G 255,255,255
//		M 0.5 D 255,255,255 S 255,0,0

static bool	parse_read_ptr(const char *str, int *current, t_shared_pt *(**
	read_ptr)(const char *str, int *current))
{
	if (str[*current] == 'D')
		*read_ptr = read_diffuse;
	else if (str[*current] == 'E')
		*read_ptr = read_emissive;
	else if (str[*current] == 'M')
		*read_ptr = read_mix;
	else if (str[*current] == 'A')
		*read_ptr = read_additive;
	else if (str[*current] == 'C')
		*read_ptr = read_checkerboard;
	else if (str[*current] == 'P')
		*read_ptr = read_perfect_mirror;
	else if (str[*current] == 'S')
		*read_ptr = read_specular;
	else
	{
		set_error("Failed to detect type of material!", false);
		return (false);
	}
	return (true);
}

// Try default material
// Get what type of material it is
// Read the material

t_shared_pt	*read_material(const char *str, int *current)
{
	t_color_hdr	color;
	t_shared_pt	*(*read_ptr)(const char *str, int *current);

	if (read_color(str, current, false, &color))
		return (read_default(&color));
	if (!parse_read_ptr(str, current, &read_ptr))
		return (NULL);
	*current += 1;
	skip_whitespace(str, current);
	return (read_ptr(str, current));
}
