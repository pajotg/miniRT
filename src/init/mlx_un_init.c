/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_un_init.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:23:03 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:46:35 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_init.h"
//#include "ft_error.h"
#include "mlx.h"

void	mlx_data_un_init(t_mlx_data *data)
{
	renderer_un_init(&data->renderer);
	image_un_init(data->mlx, &data->img);
	if (data->window)
		mlx_destroy_window(data->mlx, data->window);
}
