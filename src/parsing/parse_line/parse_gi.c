/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_gi.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:26:17 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/11 21:57:09 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_scene.h"
//#include "mini_rt_object_data.h"
//#include "mini_rt_ray_intersects.h"
//#include "mini_rt_object.h"
#include "ft_error.h"
//#include <stdlib.h>
#include "libft.h"
//#include "ft_parse_utils.h"
//#include "mini_rt_parse_utils.h"

bool	scene_parse_gi(t_scene *scene, char *line, int *curr)
{
	if (scene->use_gi)
	{
		set_error(ft_strjoin("Duplicate GI: ", line), true);
		return (false);
	}
	scene->use_gi = true;
	(void)curr;
	return (true);
}
