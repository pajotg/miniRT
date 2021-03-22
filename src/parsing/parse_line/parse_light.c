/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_light.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:37:48 by jasper        #+#    #+#                 */
/*   Updated: 2021/02/05 12:18:54 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_object.h"
#include "ft_error.h"
#include <stdlib.h>
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"

bool	scene_parse_light(t_scene *scene, char *line, int *curr)
{
	t_light	light;

	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, ',', &light.position))
	{
		set_error(ft_strjoin(
				"Light position incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_color(line, curr, true, &light.color))
	{
		set_error(ft_strjoin(
				"Light color incorrectly formatted: ", line), true);
		return (false);
	}
	if (!list_push(&scene->lights, &light))
	{
		set_error("Could not push light into lights list!", true);
		return (false);
	}
	return (true);
}
