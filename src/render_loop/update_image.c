/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   update_image.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:47:39 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:19:02 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

void write_pix(t_mlx_data* data, int x, int y, t_color_hdr hdr)
{
	t_color_rgb rgb = color_hdr_to_rgb_reindard_white(hdr, data->white);
	unsigned int col = (rgb.b | rgb.g << 8) | rgb.r << 16;

	size_t offset = x * (data->img.bits_per_pixel / 8) + y * data->img.line_length;
	unsigned int* addr = (unsigned int*)(data->img.addr + offset);
	*addr = col;
}

void update_pix(t_mlx_data* data, int x, int y)
{
	t_color_hdr hdr = get_hdr(data, x, y);
	write_pix(data, x, y, hdr);
}

void update_image(t_mlx_data* data)
{
	for (int x = 0; x < data->img.width; x++)
		for (int y = 0; y < data->img.height; y++)
			update_pix(data, x, y);
}
