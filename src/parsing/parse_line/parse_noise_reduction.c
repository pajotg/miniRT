/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_noise_reduction.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:26:17 by jasper        #+#    #+#                 */
/*   Updated: 2021/03/22 15:21:04 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_scene.h"

bool	scene_parse_noise_reduction(t_scene *scene, char *line, int *curr)
{
	if (scene->noise_reduction != 0)
	{
		set_error(ft_strjoin("Duplicate noise reduction: ", line), true);
		return (false);
	}
	skip_whitespace(line, curr);
	if (!read_int(line, curr, &scene->noise_reduction) || scene
		->noise_reduction <= 0)
	{
		set_error(ft_strjoin("noise reduction incorrectly formatted: ", line),
			true);
		return (false);
	}
	return (true);
}
