#ifndef STUPID_NORM_H
# define STUPID_NORM_H

# include "mini_rt_mlx.h"
# include "mini_rt_hooks.h"
# include "mini_rt_render_pixel.h"
# include "mini_rt_init.h"
# include "mini_rt_object.h"

# include <stdlib.h>
# include "ft_printf.h"
# include "mini_rt_bmp.h"
# include "libft.h"
# include "mlx.h"

# ifdef OS_Linux
#  include "mlx_int.h"
# endif

# include <math.h>
# include "ft_printf.h"
# include "ft_error.h"
# include "ft_time.h"	// Used for movement (bonus)
# include "ft_manual_reset_event.h"
# include <stdio.h>

// ROFL
void	mlx_destroy_display_but_only_on_linux(void *ptr);

void	cap_resolution(void *mlx, t_scene *scene);
void	do_loop(t_mlx_data *data);
void	hook_events(t_mlx_data *mlx_data);

#endif