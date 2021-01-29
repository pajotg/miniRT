/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:11:07 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/29 13:51:20 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "ft_parse_utils.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include "mini_rt_material_data.h"

bool read_transform(const char* str, int* current, t_transform *o_transform)
{
	if (!read_vec3(str, current, &o_transform->position))
		return false;
	skip_whitespace(str, current);
	t_vec3 forward;
	if (!read_vec3_unit(str, current, &forward))
		return false;
	float dot = vec3_dot(&forward, vec3_up());
	if ( dot > 0.999 || dot < -0.999)
		quaternion_from_forward_up(&o_transform->rotation, &forward, vec3_right());
	else
		quaternion_from_forward_up(&o_transform->rotation, &forward, vec3_up());
	return true;
}

bool read_color(const char* str, int* current, bool has_ratio, t_color_hdr *o_color)
{
	int read;
	float ratio;
	if (has_ratio)
	{
		if (!read_float(str, current, &ratio))
			return false;
		if (ratio < 0 || ratio > 1)
			return false;
		ratio /= 255;
		skip_whitespace(str, current);
	}
	else
		ratio = 1 / 255.0;

	if (!read_int(str, current, &read))
		return false;
	o_color->r = read * ratio;
	if (!skip_char(str, current, ',') || read < 0 || read > 255)
		return false;

	if (!read_int(str, current, &read))
		return false;
	o_color->g = read * ratio;
	if (!skip_char(str, current, ',') || read < 0 || read > 255)
		return false;

	if (!read_int(str, current, &read))
		return false;
	o_color->b = read * ratio;
	if (read < 0 || read > 255)
		return false;

	return true;
}

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

t_shared_pt8 *read_material(const char* str, int* current)
{
	// A material is either
	// a plain color, aka: diffuse only
	// Defined by a tree like structure:
	// M value [material] [material]	// mix
	// A [material] [material]	// add
	// D color	// diffuse
	// S color roughness	// specular
	// G color roughness	// glossy
	// T color	// transparent
	// examples of valid materials:
	//	255,255,255
	//	G 255,255,255
	//	M 0.5 D 255,255,255 S 255,0,0

	t_color_hdr color;
	t_material material;
	if (read_color(str, current, false, &color))
	{
		material_diffuse_init(&material, &color);
		return (create_shared_ptr_from_material(&material));
	}

	set_error("Failed to read material!", false);
	return (NULL);
}