/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_mlx.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:24:52 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 12:10:51 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_MLX_H
# define MINI_RT_MLX_H

# include "mini_rt_scene.h"
# include "mini_rt_args.h"
# include "mini_rt_renderer.h"

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
	void				*mlx;
	void				*window;
	t_scene				*scene;
	t_input				input;
	t_mlx_image			img;
	t_args				*args;
	float				white;
	bool				should_clear;
	bool				active;

	t_pixel_renderer	renderer;
}				t_mlx_data;

void			correct_exit(t_mlx_data *data);
void			update_image(t_mlx_data *data);
t_color_hdr		get_hdr(t_mlx_data *data, int x, int y);

#endif
