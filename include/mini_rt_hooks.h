/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_hooks.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 17:46:54 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 14:13:15 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_HOOKS_H
# define MINI_RT_HOOKS_H

int	hook_key_down(int key,void *p);
int	hook_key_up(int key,void *p);
int hook_mouse(int button, int x, int y, void* p);
int hook_client_message(void* p);

#endif
