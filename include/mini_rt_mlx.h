/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_mlx.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 18:51:02 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 11:38:21 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MLX_H
# define MINI_RT_MLX_H

# include <pthread.h>

typedef struct	s_pixel_data
{
	t_color_hdr color;
}				t_pixel_data;

typedef struct	s_mlx_image
{
	void	*image;
	char	*addr;
	int		width;
	int		height;

	int		bits_per_pixel;
	int		line_length;
	bool	big_endian;
}				t_mlx_image;

typedef struct	s_input
{
	bool left;
	bool right;
	bool forward;
	bool backward;
	bool up;
	bool down;

	bool white_up;
	bool white_down;
}				t_input;

typedef struct	s_mlx_data
{
	void			*mlx;
	void			*window;
	t_scene			*scene;
	t_input			input;
	t_mlx_image		img;
	float			white;
	bool			active;
	pthread_mutex_t	lock;

	int				current_pixel;
	t_pixel_data	*pixels;
}				t_mlx_data;

#endif
