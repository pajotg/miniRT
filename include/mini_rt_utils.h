/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_utils.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 11:26:48 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/03 12:07:37 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_UTILS_H
# define MINI_RT_UTILS_H

char			*get_last_error();
void			set_error(char *error_msg, bool should_free);
void			clear_error();

#endif
