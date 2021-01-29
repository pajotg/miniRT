/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_mix.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/29 15:05:16 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include <math.h>

static void material_mix(const t_scene* scene, const void* material_data, const t_material_trace_data* trace_data, t_color_hdr* o_hdr)
{
	const t_material_mix* mix = material_data;
	t_material* a = mix->a->ptr;
	t_material* b = mix->b->ptr;

	t_color_hdr tmp;
	a->material_func(scene, a->material_data, trace_data, o_hdr);
	o_hdr->r *= 1 - mix->ratio;
	o_hdr->g *= 1 - mix->ratio;
	o_hdr->b *= 1 - mix->ratio;
	b->material_func(scene, b->material_data, trace_data, &tmp);
	o_hdr->r += tmp.r * mix->ratio;
	o_hdr->g += tmp.g * mix->ratio;
	o_hdr->b += tmp.b * mix->ratio;
}

static void material_mix_free(void* material_data)
{
	t_material_mix* mix = material_data;
	shared_pt8_release_and_free(mix->a);
	shared_pt8_release_and_free(mix->b);
	free(material_data);
}

bool material_mix_init(t_material* material, float ratio, t_shared_pt8* a, t_shared_pt8 *b)
{
	t_material_mix* mix = malloc(sizeof(t_material_mix));
	if (mix == NULL)
		return (false);
	mix->ratio = ratio;
	shared_pt8_get(a);
	shared_pt8_get(b);
	mix->a = a;
	mix->b = b;

	*material = (t_material)
	{
		.material_data = mix,
		.material_func = material_mix,
		.material_free_data_func = material_mix_free
	};
	return (true);
}