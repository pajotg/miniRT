/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_obj.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/16 17:31:01 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/17 13:33:40 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
#include "libft.h"
#include "mini_rt_parse_utils.h"
#include "get_next_line.h"
#include "ft_vec2.h"
#include <stdbool.h>
#include <stdlib.h>

static bool is_type(char *line, char *object, int *curr)
{
	int i;

	i = *curr;
	while (*object)
	{
		if (line[i] != *object)
			return (false);
		object++;
		i++;
	}
	if (!ft_isspace(line[i]))
		return (false);
	*curr = i + 1;
	return (true);
}

static bool parse_line(t_scene_parse_data *parse_data, t_scene *scene, char *line)
{
	int curr;

	if (line[0] == '\0' || line[0] == '#')
		return (true);
	curr = 0;

	if (is_object(line, "v", curr))	// vertex
		return parse_resolution(parse_data, scene, line, curr);
	else if (is_object(line, "vt", curr))	// texture coordinate
		return parse_ambiant(parse_data, scene, line, curr);
	else if (is_object(line, "vn", curr))	// vertex normal
		return parse_ambiant(parse_data, scene, line, curr);
	else
	{
		// type: vp, parameter space vertices
		set_error(ft_strjoin("Unknown line type: ", line), true);
		return (false);
	}

	skip_whitespace(line, &curr);
	if (line[curr] != '\0')
	{
		set_error(ft_strjoin("Line contains more data than expected: ", line), true);
		return false;
	}
	return true;
}

void free_obj(t_obj* obj)
{
	list_un_init(&(obj->faces), NULL);
	free(obj);
}

t_obj* parse_obj_file(int fd)
{
	t_obj* obj = malloc(sizeof(t_obj));
	if (obj == NULL)
	{
		set_error("Malloc failed in parse_obj_file!", false);
		return NULL;
	}
	t_list vertexes;
	t_list normals;
	t_list uvs;

	bool init_success;
	init_success = list_init(&obj->faces, sizeof(t_face));
	init_success = list_init(&vertexes, sizeof(t_face)) && init_success;
	init_success = list_init(&normals, sizeof(t_face)) && init_success;
	init_success = list_init(&uvs, sizeof(t_face)) && init_success;
	if (!init_success)
	{
		list_un_init(&vertexes, NULL);
		list_un_init(&normals, NULL);
		list_un_init(&uvs, NULL);
		free_obj(obj);
		set_error("Malloc failed in list init!", false);
		return NULL;
	}

	char* line;
	while (true)
	{
		int out = get_next_line(fd, &line);
		if (out == -1)
		{
			set_error(ft_strjoin("Could not read file: ", get_last_error()), true);
			clear_gnl_data(fd);
			free_obj(obj);
			return (NULL);
		}

		free(line);
		if (out == 0)
			break;
	}
	return (obj);
}
*/