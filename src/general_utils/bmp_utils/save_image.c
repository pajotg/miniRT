/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   save_image.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 14:03:09 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:31:35 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bmp.h"
#include "mini_rt_mlx.h"
#include "ft_error.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ft_init.h"

// (rgb.b | rgb.g << 8) | rgb.r << 16;

static void	write_image_to_pixels(t_mlx_image *img, unsigned char *pixels)
{
	int				i;
	size_t			offset;
	unsigned int	col;
	int				x;
	int				y;

	i = 0;
	while (i < img->width * img->height)
	{
		x = i % img->width;
		y = i / img->width;
		offset = x * (img->bits_per_pixel / 8) + y * img->line_length;
		col = *(unsigned int *)(img->addr + offset);
		pixels[i * 3] = (col >> 16) & 0xff;
		pixels[i * 3 + 1] = (col >> 8 ) & 0xff;
		pixels[i * 3 + 2] = (col >> 0 ) & 0xff;
		i++;
	}
}

// returns bit inverted error index,

bool	save_image(t_mlx_image *img, char *path)
{
	t_init_data		init_data[2];
	unsigned char	*pixels;
	int				fd;

	init_data[0] = init_data_create_malloc(&pixels,
			img->width * img->height * 3, "Could not malloc pixels to create sc"
			"reenshot!");
	init_data[1] = init_data_create_open(&fd, path,
			O_WRONLY | O_CREAT | O_TRUNC, "Could not open/create screenshot fil"
			"e!");
	if (init_values(2, init_data))
		return (false);
	write_image_to_pixels(img, pixels);
	if (!write_bmp(fd, pixels, img->width, img->height))
	{
		free(pixels);
		close(fd);
		set_error(ft_strjoin("Could not write into file: ", path), true);
		return (false);
	}
	free_values(2, init_data);
	return (true);
}
