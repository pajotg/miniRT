/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_light.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:37:48 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/27 16:40:45 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"

bool	parse_light(t_scene *scene, char *line, int *curr)
{
	t_light light;

	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, &light.position))
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
	darray_push(&scene->lights, &light);
	return (true);
}
