/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_utils.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:11:44 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:58:26 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_UTILS_H
# define MINI_RT_PARSE_UTILS_H

# include "mini_rt_object.h"

bool			read_transform(char *str, int *current, t_transform *transform);
bool			read_color(char *str, int *current, bool has_ratio,
	t_color_hdr *color);

#endif
