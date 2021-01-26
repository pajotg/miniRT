/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_anti_aliasing.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:26:17 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 18:30:31 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"
#include "mini_rt_object.h"
#include "ft_error.h"
#include <stdlib.h>
#include "libft.h"
#include "ft_parse.h"
#include "mini_rt_parse_utils.h"

bool	parse_anti_aliasing(t_scene_parse_data *parse_data,
	t_scene *scene, char *line, int *curr)
{
	if (parse_data->has_anti_aliasing)
	{
		set_error(ft_strjoin("Duplicate anti aliasing: ", line), true);
		return (false);
	}
	parse_data->has_anti_aliasing = true;
	skip_whitespace(line, curr);
	if (!read_int(line, curr, &scene->samples_per_pixel) || scene->samples_per_pixel < 0)
	{
		set_error(ft_strjoin("anti aliasing incorrectly formatted: ", line), true);
		return (false);
	}
	return (true);
}
