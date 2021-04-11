/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_camera.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:33:48 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/11 21:56:34 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//#include "mini_rt_object.h"
#include "ft_error.h"
//#include <stdlib.h>
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_utils.h"
#include <math.h>

static bool	stupid_norm(t_camera *camera, char *line, int *curr)
{
	skip_whitespace(line, curr);
	if (!read_float(line, curr, &camera->fov))
	{
		set_error(ft_strjoin("Camera missing FOV: ", line), true);
		return (false);
	}
	if (camera->fov < 0 || camera->fov > 180)
	{
		set_error(ft_strjoin("Camera FOV out of range! (0-180): ", line), true);
		return (false);
	}
	camera->fov = camera->fov / 180 * M_PI;
	return (true);
}

bool	scene_parse_camera(t_scene *scene, char *line, int *curr)
{
	t_camera	camera;

	skip_whitespace(line, curr);
	if (!read_transform(line, curr, &camera.transform))
	{
		set_error(ft_strjoin(
				"Camera transform incorrectly formatted: ", line), true);
		return (false);
	}
	if (!stupid_norm(&camera, line, curr))
		return (false);
	if (!list_push(&scene->cameras, &camera))
	{
		set_error("Could not push camera into cameras list!", true);
		return (false);
	}
	return (true);
}
