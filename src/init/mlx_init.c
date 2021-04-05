/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:23:03 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 14:56:56 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_init.h"
#include "ft_error.h"
#include "mlx.h"

#include "ft_ternary.h"

static void	init_data(t_mlx_data *data, void *mlx, t_scene *scene, t_args *args)
{
	data->mlx = mlx;
	data->scene = scene;
	data->args = args;
	data->should_clear = false;
	data->white = ter_int(scene->use_gi, 5, 1);
	data->active = true;
	data->should_auto_white = false;
	data->should_update_screen = false;
	data->input = (t_input){false, false, false, false, false, false, false,
		false, false};
}

static bool	init_window(t_mlx_data *data)
{
	if (!data->args->save)
	{
		data->window = mlx_new_window(data->mlx, data->scene->resolution.width,
				data->scene->resolution.height, "miniRT");
		if (!data->window)
		{
			set_error("Could not create new mlx window!", false);
			return (false);
		}
	}
	else
		data->window = NULL;
	return (true);
}

bool	mlx_data_init(t_mlx_data *data, void *mlx, t_scene *scene, t_args *args)
{
	init_data(data, mlx, scene, args);
	init_window(data);
	image_init(mlx, &data->img, scene->resolution.width, scene
		->resolution.height);
	if (!data->img.image)
	{
		if (data->window)
			mlx_destroy_window(data->mlx, data->window);
		set_error("Could not create mlx image!", false);
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
