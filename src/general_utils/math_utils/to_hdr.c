/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   to_hdr.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:50:08 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 16:51:19 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

t_color_hdr convert_to_hdr(t_pixel_data* pixel)
{
	t_color_hdr out;
	if (pixel->num_samples == 0)
	{
		out = (t_color_hdr) { 0,0,0 };
		return (out);
	}
	out.r = pixel->color.r / pixel->num_samples;
	out.g = pixel->color.g / pixel->num_samples;
	out.b = pixel->color.b / pixel->num_samples;
	return (out);
}

t_color_hdr get_hdr(t_mlx_data *data, int x, int y)
{
	return (convert_to_hdr(&data->renderer.pixels[x + y * data->scene->resolution.width]));
}