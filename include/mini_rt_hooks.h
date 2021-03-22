/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_hooks.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:18:33 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 15:04:12 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_HOOKS_H
# define MINI_RT_HOOKS_H

# include "mini_rt_mlx.h"

int		hook_key_down(int key, void *p);
int		hook_key_up(int key, void *p);
int		hook_mouse(int button, int x, int y, void *p);
int		hook_client_message(void *p);

void	hook_frame_start(t_mlx_data *data);
void	hook_frame_complete(t_mlx_data *mlx, int total_samples, float avg_noise)
		;

int		hook_loop(void *p);

#endif
