/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   edge_detect.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:48:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 14:06:24 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

float	get_difference(t_color_hdr *col_a, t_color_hdr *col_b)
{
	float	r;
	float	g;
	float	b;

	r = col_a->r - col_b->r;
	g = col_a->g - col_b->g;
	b = col_a->b - col_b->b;
	return (r * r + g * g + b * b);
}

//edgyness = edgyness / sqrtf(1 + edgyness * edgyness);
// Apply a sigmoid function to map it to a 0-1 range
//exit(1);
//printf("Got edgyness: %.2f for pixel: %i, %i\n", edgyness, x, y);
//printf("	other hdr: %.2f %.2f %.2f\n", other.r, other.g, other.b);
//printf("	Got edgyness: %.2f for pixel: %i, %i\n", edgyness, x + ox, y + oy);
//printf("center hdr: %.2f %.2f %.2f\n", center.r, center.g, center.b);

static int	inc(int *value)
{
	(*value)++;
	return (*value);
}

float	get_edgyness(t_mlx_data *data, int x, int y)
{
	int			oy;
	int			ox;
	float		edgyness;
	t_color_hdr	other;
	t_color_hdr	center;

	edgyness = 0;
	center = get_hdr(data, x, y);
	ox = -2;
	while (inc(&ox) <= 1)
	{
		if (x + ox < 0 || x + ox >= data->scene->resolution.width)
			continue ;
		oy = -2;
		while (inc(&oy) <= 1)
		{
			if (y + oy < 0 || y + oy >= data->scene->resolution.height || (
					ox == 0 && oy == 0))
				continue ;
			other = get_hdr(data, x + ox, y + oy);
			edgyness += get_difference(&center, &other);
		}
	}
	return (edgyness);
}
