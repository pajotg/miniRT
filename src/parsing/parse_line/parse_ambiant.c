/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_ambiant.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:26:17 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/08 16:21:53 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include "ft_error.h"

#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>

bool	parse_ambiant(t_scene_parse_data *parse_data,
	t_scene *scene, char *line, int *curr)
{
	if (parse_data->has_ambiant)
	{
		set_error(ft_strjoin("Duplicate ambiant: ", line), true);
		return (false);
	}
	parse_data->has_ambiant = true;
	skip_whitespace(line, curr);
	if (!read_color(line, curr, true, &scene->ambiant))
	{
		set_error(ft_strjoin("Ambiant incorrectly formatted: ", line), true);
		return (false);
	}

	return (true);
}
