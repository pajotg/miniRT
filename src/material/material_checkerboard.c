/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_checkerboard.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:25:35 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 14:09:22 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material_data.h"
#include "mini_rt_object.h"
#include <stdlib.h>
#include <math.h>

#define M_EPSILON 0.001

static int	get_checker(float value)
{
	value += M_EPSILON;
	if (value < 0)
		return ((int)value + 1);
	return ((int)value);
}

static void	material_checkerboard(const t_scene *scene, const void *
	material_data, const t_material_trace_data *trace_data, t_color_hdr *o_hdr)
{
	t_material						*mat;
	int								checker;
	const t_material_checkerboard	*checkerboard = material_data;

	mat = checkerboard->mat->ptr;
	mat->material_func(scene, mat->material_data, trace_data, o_hdr);
	checker = 0;
	checker += get_checker(trace_data->hit->location.x * checkerboard
			->i_checker_size);
	checker += get_checker(trace_data->hit->location.y * checkerboard
			->i_checker_size);
	checker += get_checker(trace_data->hit->location.z * checkerboard
			->i_checker_size);
	if (checker & 1)
	{
		o_hdr->r *= checkerboard->color.r;
		o_hdr->g *= checkerboard->color.g;
		o_hdr->b *= checkerboard->color.b;
	}
}

static void	material_checkerboard_free(void *material_data)
{
	t_material_checkerboard	*checkerboard;

	checkerboard = material_data;
	shared_pt_release_and_free(checkerboard->mat);
	free(material_data);
}

bool	material_checkerboard_init(t_material *material, float checker_size,
	const t_color_hdr *color, t_shared_pt *mat)
{
	t_material_checkerboard	*checkerboard;

	checkerboard = malloc(sizeof(
				t_material_checkerboard));
	if (checkerboard == NULL)
		return (false);
	shared_pt_get(mat);
	checkerboard->i_checker_size = 1 / checker_size;
	checkerboard->color = *color;
	checkerboard->mat = mat;
	*material = (t_material)
	{
		.material_data = checkerboard,
		.material_func = material_checkerboard,
		.material_free_data_func = material_checkerboard_free
	};
	return (true);
}
