/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_directional_light.c                          :+:    :+:            */
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

bool	scene_parse_directional_light(t_scene *scene, char *line, int *curr)
{
	t_directional_light light;

	skip_whitespace(line, curr);
	if (!read_vec3_unit(line, curr, ',', &light.direction))
	{
		set_error(ft_strjoin(
			"Directional light direction incorrectly formatted: ", line), true);
		return (false);
	}
	// I really need an invert function
	light.direction = (t_vec3) { -light.direction.x,
		-light.direction.y, -light.direction.z };
	skip_whitespace(line, curr);
	if (!read_color(line, curr, true, &light.color))
	{
		set_error(ft_strjoin(
			"Directional light color incorrectly formatted: ", line), true);
		return (false);
	}
	if (!list_push(&scene->directional_lights, &light))
	{
		set_error("Could not push Directional light into directional_lights list!", true);
		return (false);
	}
	return (true);
}
