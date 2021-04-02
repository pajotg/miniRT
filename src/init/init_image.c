/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_image.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:27:01 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/02 14:01:03 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mlx.h"

bool	image_init(void *mlx, t_mlx_image *img, int width, int height)
{
	void	*image;

	image = mlx_new_image(mlx, width, height);
	if (!image)
		return (false);
	img->image = image;
	img->width = width;
	img->height = height;
	img->addr = mlx_get_data_addr(image, &img->bits_per_pixel,
			&img->line_length, (int *)&img->big_endian);
	return (true);
}

void	image_un_init(void *mlx, t_mlx_image *img)
{
	mlx_destroy_image(mlx, img->image);
}
