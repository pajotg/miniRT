/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_resulution.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:17:21 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 18:29:06 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "libft.h"
#include "ft_parse.h"
#include "mini_rt_parse_utils.h"

bool	parse_resolution(t_scene_parse_data *parse_data,
	t_scene *scene, char *line, int *curr)
{
	if (parse_data->has_resolution)
	{
		set_error(ft_strjoin("Duplicate resolution: ", line), true);
		return (false);
	}
	parse_data->has_resolution = true;
	skip_whitespace(line, curr);
	if (!read_int(line, curr, &scene->resolution.width))
	{
		set_error(ft_strjoin("resolution missing width: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_int(line, curr, &scene->resolution.height))
		set_error(ft_strjoin("resolution missing height: ", line), true);
	else if (scene->resolution.width <= 0 || scene->resolution.height <= 0)
		set_error("Invalid resolution!", false);
	else
		return (true);
	return (false);
}
