/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks_input.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/03 14:11:21 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/22 15:50:48 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_object.h"
#include <stdio.h>
#include <math.h>
#include "ft_printf.h"
#include "mini_rt_render_pixel.h"
// TODO: Mac keycodes

#ifdef OS_Linux
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_Q 113
# define KEY_E 101
# define KEY_T 116
# define KEY_Z 122
# define KEY_P 112
# define KEY_O 111

# define KEY_R 114
# define KEY_F 102
#else
# define KEY_ESC 53
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_Q 12
# define KEY_E 14
# define KEY_T 17
# define KEY_Z 6
# define KEY_P 35
# define KEY_O 31

# define KEY_R 15
# define KEY_F 3
#endif

static void	next_cam(t_mlx_data *data)
{
	size_t	index;

	index = data->scene->current_camera_index + 1;
	if (index >= data->scene->cameras.count)
		index = 0;
	data->scene->current_camera_index = index;
	data->should_clear = true;
}

//printf("Key down: %i\n", key);

static void	secondairy_hook_key_down_if_else_case(t_mlx_data *data, int key)
{
	if (key == KEY_P)
		data->should_auto_white = true;
	else if (key == KEY_O)
		data->input.camera_free = !data->input.camera_free;
	else if (key == KEY_T)
		next_cam(data);
}

// this is dumb
//printf("key: %i\n", key);

int	hook_key_down(int key, void *p)
{
	t_mlx_data	*data;

	data = p;
	if (key == KEY_ESC)
		correct_exit(data);
	else if (key == KEY_W)
		data->input.forward = true;
	else if (key == KEY_A)
		data->input.left = true;
	else if (key == KEY_S)
		data->input.backward = true;
	else if (key == KEY_D)
		data->input.right = true;
	else if (key == KEY_Q)
		data->input.up = true;
	else if (key == KEY_E)
		data->input.down = true;
	else if (key == KEY_R)
		data->input.white_up = true;
	else if (key == KEY_F)
		data->input.white_down = true;
	else
		secondairy_hook_key_down_if_else_case(data, key);
	return (0);
}

//printf("Key up: %i\n", key);

int	hook_key_up(int key, void *p)
{
	t_mlx_data	*data;

	data = p;
	if (key == KEY_W)
		data->input.forward = false;
	else if (key == KEY_A)
		data->input.left = false;
	else if (key == KEY_S)
		data->input.backward = false;
	else if (key == KEY_D)
		data->input.right = false;
	else if (key == KEY_Q)
		data->input.up = false;
	else if (key == KEY_E)
		data->input.down = false;
	else if (key == KEY_R)
		data->input.white_up = false;
	else if (key == KEY_F)
		data->input.white_down = false;
	return (0);
}

int	hook_client_message(void *p)
{
	t_mlx_data	*data;

	data = p;
	correct_exit(data);
	return (0);
}
