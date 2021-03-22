/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_anti_aliasing.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:26:17 by jasper        #+#    #+#                 */
/*   Updated: 2021/02/05 12:53:50 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"
#include "mini_rt_object.h"
#include "ft_error.h"
#include <stdlib.h>
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"
#include "ft_list.h"

bool	scene_parse_anti_aliasing(t_scene *scene, char *line, int *curr)
{
	if (scene->samples_per_pixel.count != 0)
	{
		set_error(ft_strjoin("Duplicate anti aliasing: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	while (true)
	{
		int spp;
		if (!read_int(line, curr, &spp) || spp <= 0)
		{
			set_error(ft_strjoin("anti aliasing incorrectly formatted: ", line), true);
			return (false);
		}
		if (!list_push(&scene->samples_per_pixel, &spp))
		{
			set_error("Failed to push AA into list!", false);
			return (false);
		}
		if (!skip_char(line, curr, ','))
			break;
	}
	return (true);
}
