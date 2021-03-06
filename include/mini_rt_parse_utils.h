/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_utils.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:11:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/20 15:44:42 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_UTILS_H
# define MINI_RT_PARSE_UTILS_H

# include "mini_rt_object.h"
# include "ft_shared_ptr.h"

bool			read_transform(const char *str, int *current,
					t_transform *transform);
bool			read_color(const char *str, int *current, bool has_ratio,
					t_color_hdr *color);
t_shared_pt		*read_material(const char *str, int *current);

#endif
