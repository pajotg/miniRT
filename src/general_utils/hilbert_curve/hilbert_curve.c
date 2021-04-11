/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hilbert_curve.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 10:15:09 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 22:08:31 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_hilbert_randomizer.h"
#include "stupid_norm.h"

static void	rot(int n, int rx, int ry, t_struct_abuse struct_abuse)
{
	int	tmp;

	if (ry == 0)
	{
		if (rx == 1)
		{
			*struct_abuse.o_cx = n - 1 - *struct_abuse.o_cx;
			*struct_abuse.o_cy = n - 1 - *struct_abuse.o_cy;
		}
		tmp = *struct_abuse.o_cx;
		*struct_abuse.o_cx = *struct_abuse.o_cy;
		*struct_abuse.o_cy = tmp;
	}
}

void	hilbert_curve_init(t_hilbert_curve *curve, int recursion)
{
	curve->recursion = recursion;
	curve->size = 1 << recursion;
	curve->num_indexes = 1 << (recursion * 2);
}

void	hilbert_curve_get(int index, t_hilbert_curve *curve, int *o_x, int *o_y)
{
	int	t;
	int	i;
	int	rx;
	int	ry;

	t = index;
	i = 1;
	*o_x = 0;
	*o_y = 0;
	while (i < curve->size)
	{
		rx = 1 & (t >> 1);
		ry = 1 & (t ^ rx);
		rot(i, rx, ry, (t_struct_abuse){o_x, o_y});
		*o_x += i * rx;
		*o_y += i * ry;
		t = t >> 2;
		i *= 2;
	}
}
