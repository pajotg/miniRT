/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_camera.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:33:48 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/27 16:36:20 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_line.h"
#include <math.h>

bool	parse_camera(t_scene *scene, char *line, int *curr)
{
	t_camera camera;

	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &camera.transform))
	{
		set_error(ft_strjoin(
			"Camera transform incorrectly formatted: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_float(line, curr, &camera.fov))
	{
		set_error(ft_strjoin("Camera missing FOV: ", line), true);
		return (false);
	}
	if (camera.fov < 0 || camera.fov > 180)
	{
		set_error(ft_strjoin("Camera FOV out of range! (0-180): ", line), true);
		return (false);
	}
	camera.fov = camera.fov / 180 * M_PI;
	darray_push(&scene->cameras, &camera);
	return (true);
}
