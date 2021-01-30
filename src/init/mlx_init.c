/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:23:03 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 18:56:43 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_init.h"
#include "ft_error.h"
#include "mlx.h"

bool	mlx_data_init(t_mlx_data* data, void* mlx, t_scene* scene, t_args* args)
{
	data->mlx = mlx;
	data->scene = scene;
	data->args = args;
	data->should_clear = false;
	data->white = 15;
	data->active = true;
	data->input = (t_input) { false, false, false, false, false, false, false, false };

	if (!data->args->save)
	{
		data->window = mlx_new_window(mlx, scene->resolution.width, scene->resolution.height, "miniRT");
		if (!data->window)
		{
			set_error("Could not create new mlx window!",false);
			return (false);
		}
	}
	else
		data->window = NULL;

	image_init(mlx, &data->img, scene->resolution.width, scene->resolution.height);
	if (!data->img.image)
	{
		if (data->window)
			mlx_destroy_window(data->mlx, data->window);
		set_error("Could not create mlx image!",false);
		return (false);
	}

	if (!renderer_init(data))
	{
		mlx_destroy_image(data->mlx, data->img.image);
		if (data->window)
			mlx_destroy_window(data->mlx, data->window);
		return (false);
	}
	update_image(data);
	return (true);
}