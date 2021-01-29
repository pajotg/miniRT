/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_additive.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/29 14:38:41 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include <math.h>

static void material_additive(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	const t_material_additive* additive = material_data;
	t_material* a = additive->a->ptr;
	t_material* b = additive->b->ptr;

	t_color_hdr tmp;
	a->material_func(scene, a->material_data, trace_data, o_hdr);
	b->material_func(scene, b->material_data, trace_data, &tmp);
	o_hdr->r += tmp.r;
	o_hdr->g += tmp.g;
	o_hdr->b += tmp.b;
}

static void material_additive_free(void* material_data)
{
	t_material_additive* additive = material_data;
	shared_pt8_release_and_free(additive->a);
	shared_pt8_release_and_free(additive->b);
	free(material_data);
}

bool material_additive_init(t_material* material, t_shared_pt8* a, t_shared_pt8 *b)
{
	t_material_additive* additive = malloc(sizeof(t_material_additive));
	if (additive == NULL)
		return (false);
	shared_pt8_get(a);
	shared_pt8_get(b);
	additive->a = a;
	additive->b = b;

	*material = (t_material)
	{
		.material_data = additive,
		.material_func = material_additive,
		.material_free_data_func = material_additive_free
	};
	return (true);
}