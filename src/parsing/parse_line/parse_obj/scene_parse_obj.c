/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene_parse_obj.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 12:14:41 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/02 17:07:01 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "libft.h"
#include "ft_parse_utils.h"
#include "mini_rt_parse_obj.h"
#include "mini_rt_scene.h"
#include "mini_rt_parse_utils.h"
#include "mini_rt_object_data.h"
#include "mini_rt_ray_intersects.h"

#include <stdlib.h>

#include <stdio.h>	// bad

//	malloc management
//	combine base dir and path from .rt file
// 	extract base dir from scene path
//	extract path from .rt file
// A ton of code to extract and combine the paths

static bool	get_true_path(const t_scene *scene, char *line, int *curr, char **
	true_path)
{
	int		start;
	int		end;
	char	*path;
	char	*stop;
	char	*base;

	start = *curr;
	while (!ft_isspace(line[*curr]))
		(*curr)++;
	end = *curr;
	path = ft_substr(line, start, end - start);
	stop = ft_strrchr(scene->scene_path, '/');
	if (!stop)
		stop = scene->scene_path;
	base = ft_substr(scene->scene_path, 0, stop - scene->scene_path);
	*true_path = ft_strjoin_va(3, base, "/", path);
	free(path);
	free(base);
	if (!*true_path)
	{
		set_error("Failed malloc!", false);
		return (false);
	}
	return (true);
}

static bool	stupid_norm1(char *line, int *curr, t_transform *transform)
{
	skip_whitespace(line, curr);
	if (!read_transform(line, curr, transform))
	{
		set_error(ft_strjoin("obj position and normal incorrectly formatted: ",
				line), true);
		return (false);
	}
	return (true);
}

static bool	stupid_norm2(char *line, int *curr, t_shared_pt **material, t_obj *
	obj)
{
	*material = read_material(line, curr);
	if (!*material)
	{
		free_obj(obj);
		return (false);
	}
	return (true);
}

bool	scene_parse_obj(t_scene *scene, char *line, int *curr)
{
	t_obj		*obj;
	bool		ret;
	t_transform	transform;
	t_shared_pt	*material;
	char		*true_path;

	if (!stupid_norm1(line, curr, &transform))
		return (false);
	skip_whitespace(line, curr);
	if (!get_true_path(scene, line, curr, &true_path))
		return (false);
	obj = parse_obj(true_path);
	free(true_path);
	if (!obj)
		return (false);
	skip_whitespace(line, curr);
	if (!stupid_norm2(line, curr, &material, obj))
		return (false);
	ret = insert_obj_into_scene(scene, obj, &transform, material);
	free_obj(obj);
	shared_pt_release_and_free(material);
	return (ret);
}
