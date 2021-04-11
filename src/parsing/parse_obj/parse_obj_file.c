/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_obj_file.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/16 17:31:01 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:50:46 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "mini_rt_parse_utils.h"
#include "mini_rt_parse_obj.h"
#include "ft_get_next_line.h"
//#include "ft_parse_utils.h"
//#include "ft_vec2.h"
//#include <stdbool.h>
#include <stdlib.h>
#include "ft_error.h"

void	free_obj(t_obj *obj)
{
	list_un_init(&(obj->faces), NULL);
	list_un_init(&(obj->vertexes), NULL);
	list_un_init(&(obj->normals), NULL);
	list_un_init(&(obj->uvs), NULL);
	free(obj);
}

static t_obj	*init_obj(void)
{
	bool	init_success;
	t_obj	*obj;

	obj = malloc(sizeof(t_obj));
	if (obj == NULL)
	{
		set_error("Malloc failed in parse_obj_file!", false);
		return (NULL);
	}
	init_success = list_init(&obj->faces, sizeof(t_face));
	init_success = list_init(&obj->vertexes, sizeof(t_vec3)) && init_success;
	init_success = list_init(&obj->normals, sizeof(t_vec3)) && init_success;
	init_success = list_init(&obj->uvs, sizeof(t_vec2)) && init_success;
	if (!init_success)
	{
		free_obj(obj);
		set_error("Malloc failed in list init!", false);
		return (NULL);
	}
	return (obj);
}

t_obj	*parse_obj_file(int fd)
{
	char	*line;
	t_obj	*obj;
	int		out;

	obj = init_obj();
	if (!obj)
		return (NULL);
	while (true)
	{
		out = get_next_line(fd, &line);
		if (out == -1 || !obj_parse_line(obj, line))
		{
			if (out == -1)
				set_error(ft_strjoin("Could not read file: ", get_last_error()),
					true);
			clear_gnl_data(fd);
			free_obj(obj);
			return (NULL);
		}
		free(line);
		if (out == 0)
			break ;
	}
	return (obj);
}
