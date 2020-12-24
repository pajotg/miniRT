/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_mlx.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 18:51:02 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/24 19:32:34 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MLX_H
# define MINI_RT_MLX_H

typedef struct	s_pixel_data
{
	t_color_hdr color;
}				t_pixel_data;

typedef struct	s_mlx_image
{
	void* image;
	char* addr;
	int width;
	int height;

	int bits_per_pixel;
	int line_length;
	bool big_endian;
}				t_mlx_image;

typedef struct	s_mlx_data
{
	void* mlx;
	void* window;
	t_scene* scene;
	t_mlx_image img;
	float white;

	int current_pixel;
	t_pixel_data* pixels;
}				t_mlx_data;

#endif