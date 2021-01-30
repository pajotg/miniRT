/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   edge_detect.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:48:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 12:15:10 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

float get_difference(t_color_hdr* col_a, t_color_hdr* col_b)
{
	float r = col_a->r - col_b->r;
	float g = col_a->g - col_b->g;
	float b = col_a->b - col_b->b;

	return (r * r + g * g + b * b);
}

float get_edgyness(t_mlx_data* data, int x, int y)
{
	float edgyness = 0;

	t_color_hdr center = get_hdr(data, x, y);
	t_color_hdr other;

	//printf("center hdr: %.2f %.2f %.2f\n", center.r, center.g, center.b);
	for (int ox = -1; ox <= 1; ox++)
	{
		if (x + ox < 0 || x + ox >= data->scene->resolution.width)
			continue;
		for (int oy = -1; oy <= 1; oy++)
		{
			if (y + oy < 0 || y + oy >= data->scene->resolution.height)
				continue;
			if (ox == 0 && oy == 0)
				continue;
			other = get_hdr(data, x + ox, y + oy);
			edgyness += get_difference(&center, &other);
			//printf("	Got edgyness: %.2f for pixel: %i, %i\n", edgyness, x + ox, y + oy);
			//printf("	other hdr: %.2f %.2f %.2f\n", other.r, other.g, other.b);
		}
	}

	//printf("Got edgyness: %.2f for pixel: %i, %i\n", edgyness, x, y);
	//exit(1);

	// Apply a sigmoid function to map it to a 0-1 range
	//edgyness = edgyness / sqrtf(1 + edgyness * edgyness);
	return edgyness;
}
