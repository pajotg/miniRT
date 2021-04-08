/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hilbert_randomizer.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 10:25:57 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/08 18:04:37 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_hilbert_randomizer.h"
#include <stdbool.h>

static int	yeet_index(int index, int max)
{
	int	step_size;

	step_size = max >> 2;
	return ((index & 3) * step_size + (index >> 2));
}

static void	inc_curve(t_hilbert_randomizer *curve)
{
	curve->index++;
	if (curve->index == curve->curve.num_indexes)
	{
		curve->index = 0;
		curve->stage++;
		if (curve->stage == 3)
		{
			curve->stage = 0;
			hilbert_curve_init(&curve->curve, curve->curve.recursion + 1);
		}
	}
}

// TODO: it seems i did something bad, this does not fix %100 of the bias
static bool	fix_bias(t_hilbert_randomizer *curve, t_vec2 *offset, int ox,
	int oy)
{
	float	scale;

	scale = 1.0 / curve->curve.size;
	offset->x = scale / 2;
	offset->y = offset->x;
	if (curve->stage == 1)
	{
		if (ox >= curve->curve.size - 1)
			return (true);
		offset->x += scale / 2;
	}
	else if (curve->stage == 2)
	{
		if (oy >= curve->curve.size - 1)
			return (true);
		offset->y += scale / 2;
	}
	return (false);
}

t_vec2	hilbert_randomizer_get_next(t_hilbert_randomizer *curve)
{
	float	scale;
	bool	is_biased;
	int		ox;
	int		oy;
	t_vec2	offset;

	scale = 1.0 / curve->curve.size;
	hilbert_curve_get(yeet_index(curve->index, curve->curve.num_indexes),
		&curve->curve, &ox, &oy);
	is_biased = fix_bias(curve, &offset, ox, oy);
	inc_curve(curve);
	if (is_biased)
		return (hilbert_randomizer_get_next(curve));
	return ((t_vec2)
		{
			.x = ox * scale + offset.x,
			.y = oy * scale + offset.y
		});
}
