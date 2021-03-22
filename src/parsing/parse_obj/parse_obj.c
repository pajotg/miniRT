/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_obj.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/27 17:23:50 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/05 13:36:38 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "mini_rt_parse_obj.h"
#include "ft_error.h"
#include "libft.h"

t_obj* parse_obj(char* path)
{
	t_obj* obj;

	int fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		set_error(ft_strjoin("Could not open file ", path), true);
		return (NULL);
	}

	obj = parse_obj_file(fd);
	if (!obj)
	{
		set_error(ft_strjoin("An error occured while parsing the file: ", get_last_error()), true);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (obj);
}