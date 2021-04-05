/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   update_image.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:47:39 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 13:17:01 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"

void	write_pix(t_mlx_data *data, int x, int y, t_color_hdr hdr)
{
	unsigned int	col;
	size_t			offset;
	t_color_rgb		rgb;
	unsigned int	*addr;

	rgb = color_hdr_to_rgb_reindard_white(hdr, data->white);
	col = (rgb.b | rgb.g << 8) | rgb.r << 16;
	offset = x * (data->img.bits_per_pixel / 8) + y * data
		->img.line_length;
	addr = (unsigned int *)(data->img.addr + offset);
	*addr = col;
}

void	update_pix(t_mlx_data *data, int x, int y)
{
	t_color_hdr	hdr;

	hdr = get_hdr(data, x, y);
	write_pix(data, x, y, hdr);
}

void	update_image(t_mlx_data *data)
{
	int	y;
	int	x;

	x = 0;
	while (x < data->img.width)
	{
		y = 0;
		while (y < data->img.height)
		{
			update_pix(data, x, y);
			y++;
		}
		x++;
	}
}
