#include "mini_rt_mlx.h"
#include "ft_ternary.h"
//#include <stdbool.h>
#include <math.h>

void	update_white_point(t_mlx_data *data)
{
	int		i;
	float	average_luminance;

	average_luminance = 0;
	i = 0;
	while (i < data->scene->resolution.width * data->scene->resolution.height)
	{
		average_luminance += color_hdr_to_luminance(convert_to_hdr(&data
					->renderer.pixels[i]));
		i++;
	}
	average_luminance /= data->scene->resolution.width * data->scene
		->resolution.height;
	data->white = average_luminance * 1.5;
	update_image(data);
	data->should_update_screen = true;
}

#if BONUS
void	handle_white_point(t_mlx_data *data, float diff)
{
	float	change;

	if (data->should_auto_white)
	{
		update_white_point(data);
		data->should_auto_white = false;
	}
	if (data->input.white_up != data->input.white_down)
	{
		change = powf(1.1, (ter_int(data->input.white_down, -1, 0)
					+ter_int(data->input.white_up, 1, 0)) * (diff * 2));
		data->white *= change;
		if (data->white < 0.001)
			data->white = 0.001;
		update_image(data);
		data->should_update_screen = true;
	}
}
#else

void	handle_white_point(t_mlx_data *data, float diff)
{
	(void)data;
	(void)diff;
}
#endif