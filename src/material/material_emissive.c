/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_emissive.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 13:31:02 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include <math.h>
#include "mini_rt_render_pixel.h"
#include "libft.h"

static void material_emissive(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	const t_material_emissive* emissive = material_data;
	*o_hdr = emissive->color;
	(void)scene;
	(void)trace_data;
}

bool material_emissive_init(t_material* material, const t_color_hdr* color)
{
	t_material_emissive* emissive = malloc(sizeof(t_material_emissive));
	if (emissive == NULL)
		return (false);
	emissive->color = *color;

	*material = (t_material)
	{
		.material_data = emissive,
		.material_func = material_emissive,
		.material_free_data_func = free
	};
	return (true);
}