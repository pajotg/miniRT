/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_ambiant.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:26:17 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/11 21:58:32 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//#include "mini_rt_object_data.h"
//#include "mini_rt_ray_intersects.h"
//#include "mini_rt_object.h"
#include "ft_error.h"
//#include <stdlib.h>
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"

bool	scene_parse_ambiant(t_scene_parse_data *parse_data,
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
