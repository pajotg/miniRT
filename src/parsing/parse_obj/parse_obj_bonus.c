/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_obj.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:23:50 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/22 16:49:55 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "mini_rt_parse_obj.h"
#include "ft_error.h"
#include "libft.h"

// TODO: Maybe cache the t_obj* struct for fast duplicates?

t_obj	*parse_obj(char *path)
{
	int		fd;
	t_obj	*obj;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		set_error(ft_strjoin("Could not open file ", path), true);
		return (NULL);
	}
	obj = parse_obj_file(fd);
	if (!obj)
	{
		set_error(ft_strjoin("An error occured while parsing the file: ",
				get_last_error()), true);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (obj);
}
