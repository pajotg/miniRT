/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   noise_detection.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/30 14:20:42 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 14:21:04 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

float get_noisyness(t_mlx_data* data, int x, int y)
{
	float noise = 0;

	//printf("center hdr: %.2f %.2f %.2f\n", center.r, center.g, center.b);
	for (int ox = -2; ox <= 2; ox++)
	{
		if (x + ox < 0 || x + ox >= data->scene->resolution.width)
			continue;
		for (int oy = -2; oy <= 2; oy++)
		{
			if (y + oy < 0 || y + oy >= data->scene->resolution.height)
				continue;
			noise += data->renderer.temp_pixels[x + ox + (y + oy) * data->scene->resolution.width].aa_difference;
		}
	}

	return noise;
}