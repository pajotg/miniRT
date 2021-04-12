//#include "mini_rt_mlx.h"
//#include "libft.h"
#include "mini_rt_render_pixel.h"
//#include "mini_rt_hooks.h"
//#include "mini_rt_image.h"
//#include "ft_list.h"
//#include <math.h>

// in case the rendering is done, wait!

void	*new_pixel_render_thread(void *p)
{
	t_mlx_data	*data;

	data = p;
	while (data->active)
	{
		render_next_pixels(data, 250);
		manual_reset_event_wait(&data->renderer.rendering_done_mre);
	}
	return (NULL);
}
