#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_mlx.h"
# include "libft.h"
# include "mini_rt_render_pixel.h"
# include "mini_rt_hooks.h"
# include "mini_rt_image.h"
# include "ft_list.h"
# include <math.h>

# include <stdio.h>

typedef struct s_render_pixels_data
{
	int		i;
	bool	is_first_render_thread;
	bool	is_last_render_thread;
	int		pix;
	int		stop;
}	t_render_pixels_data;

int	copy_temp_to_real(t_mlx_data *data);
int	complete_first_frame(t_mlx_data *data);
int	complete_aa_pixel(t_mlx_data *data, int i);
int	complete_aa_frame(t_mlx_data *data, float *avg_noise);

#endif