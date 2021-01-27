/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_scene.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:23:50 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/27 17:31:04 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "mini_rt_scene.h"
#include "ft_error.h"
#include "libft.h"

t_scene* parse_scene(char* path)
{
	t_scene* scene;

	int fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		set_error(ft_strjoin("Could not open file ", path), true);
		return (NULL);
	}

	scene = parse_scene_file(fd);
	if (!scene)
	{
		set_error(ft_strjoin("An error occured while parsing the file: ", get_last_error()), true);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (scene);
}