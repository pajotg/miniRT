/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   noise_detection.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/30 14:20:42 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 14:06:37 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

//printf("center hdr: %.2f %.2f %.2f\n", center.r, center.g, center.b);

static int	inc(int *value)
{
	(*value)++;
	return (*value);
}

float	get_noisyness(t_mlx_data *data, int x, int y)
{
	int		oy;
	int		ox;
	float	noise;
	int		count;

	noise = 0;
	count = 0;
	ox = -3;
	while (inc(&ox) <= 2)
	{
		if (x + ox < 0 || x + ox >= data->scene->resolution.width)
			continue ;
		oy = -3;
		while (inc(&oy) <= 2)
		{
			if (y + oy < 0 || y + oy >= data->scene->resolution.height)
				continue ;
			noise += data->renderer.temp_pixels[x + ox + (y + oy) * data
				->scene->resolution.width].aa_difference;
			count ++;
		}
	}
	return (noise / count);
}
