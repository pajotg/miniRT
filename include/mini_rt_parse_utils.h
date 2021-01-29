/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_utils.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:11:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/29 13:50:04 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_UTILS_H
# define MINI_RT_PARSE_UTILS_H

# include "mini_rt_object.h"
# include "ft_shared_ptr.h"

bool			read_transform(char *str, int *current, t_transform *transform);
bool			read_color(char *str, int *current, bool has_ratio,
	t_color_hdr *color);
t_shared_pt8	*read_material(const char* str, int* current);

#endif
