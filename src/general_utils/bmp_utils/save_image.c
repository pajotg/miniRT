/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   save_image.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 14:03:09 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/25 14:22:44 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "ft_error.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

bool save_image(t_mlx_image* img, char* path)
{
	unsigned char* pixels = malloc(img->width * img->height * 3);
	if (pixels == NULL)
	{
		set_error("Error\nCould not malloc pixels to create screenshot!\n", false);
		return false;
	}

	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
	if (fd == -1)
	{
		set_error(ft_strjoin("Could not open/create file: ", path), true);
		return false;
	}

	for (int i = 0; i < img->width * img->height; i++)
	{
		int x = i % img->width;
		int y = i / img->width;

		size_t offset = x * (img->bits_per_pixel / 8) + y * img->line_length;
		unsigned int col = *(unsigned int*)(img->addr + offset);
		// (rgb.b | rgb.g << 8) | rgb.r << 16;
		unsigned char r = (col >> 16) & 0xff;
		unsigned char g = (col >> 8 ) & 0xff;
		unsigned char b = (col >> 0 ) & 0xff;

		pixels[i * 3] = r;
		pixels[i * 3 + 1] = g;
		pixels[i * 3 + 2] = b;
	}

	if (!write_bmp(fd, pixels, img->width, img->height))
	{
		free(pixels);
		close(fd);
		set_error(ft_strjoin("Could not write into file: ", path), true);
		return false;
	}
	free(pixels);
	close(fd);

	return true;
}
