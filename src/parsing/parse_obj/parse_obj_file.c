/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_obj_file.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/16 17:31:01 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/29 15:53:18 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "mini_rt_parse_utils.h"
#include "mini_rt_parse_obj.h"
#include "ft_get_next_line.h"
#include "ft_parse_utils.h"
#include "ft_vec2.h"
#include <stdbool.h>
#include <stdlib.h>
#include "ft_error.h"

static bool	is_type(char *line, char *object, int *curr)
{
	int	i;

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

// texture_index and normal_index can be empty, and / can be missing if nothing
//
//is after
// indexes start with 1, so subtract one
// face is defined by: vertex_index/texture_index/normal_index

static bool	read_face_vertex(char *line, int *curr, t_face_vertex *
	o_face_vertex)
{
	o_face_vertex->vertex_index = 0;
	o_face_vertex->texture_index = 0;
	o_face_vertex->normal_index = 0;
	if (!read_int(line, curr, &o_face_vertex->vertex_index))
		return (false);
	if (skip_char(line, curr, '/'))
	{
		read_int(line, curr, &o_face_vertex->texture_index);
		if (skip_char(line, curr, '/'))
			read_int(line, curr, &o_face_vertex->normal_index);
	}
	o_face_vertex->vertex_index--;
	o_face_vertex->texture_index--;
	o_face_vertex->normal_index--;
	if (o_face_vertex->vertex_index < 0)
	{
		set_error("Vertex index below 0!", false);
		return (false);
	}
	return (true);
}

static bool	parse_line_v(t_obj *obj, char *line, int *curr)
{
	t_vec3	vec;

	skip_whitespace(line, curr);
	if (!read_vec3(line, curr, ' ', &vec))
		return (false);
	list_push(&obj->vertexes, &vec);
	return (true);
}

static bool	parse_line_vt(t_obj *obj, char *line, int *curr)
{
	t_vec2	vec;

	skip_whitespace(line, curr);
	if (!read_float(line, curr, &vec.x)
		|| !skip_char(line, curr, ' ')
		|| !read_float(line, curr, &vec.y))
		return (false);
	list_push(&obj->uvs, &vec);
	return (true);
}

static bool	parse_line_vn(t_obj *obj, char *line, int *curr)
{
	t_vec3	vec;

	skip_whitespace(line, curr);
	if (!read_vec3_unit(line, curr, ' ', &vec))
		return (false);
	list_push(&obj->normals, &vec);
	return (true);
}

static bool	parse_line_f(t_obj *obj, char *line, int *curr)
{
	t_face	face;

	if (!read_face_vertex(line, curr, &face.a))
		return (false);
	skip_whitespace(line, curr);
	if (!read_face_vertex(line, curr, &face.b))
		return (false);
	skip_whitespace(line, curr);
	if (!read_face_vertex(line, curr, &face.c))
		return (false);
	list_push(&obj->faces, &face);
	return (true);
}

//return (false);
//set_error(ft_strjoin("Unknown line type: ", line), true);
// type: vp, parameter space vertices, o
// face
// vertex normal
// texture coordinate
// vertex

static bool	parse_line(t_obj *obj, char *line)
{
	bool	success;
	int		curr;

	success = true;
	if (line[0] == '\0' || line[0] == '#')
		return (true);
	curr = 0;
	if (is_type(line, "v", &curr))
		success = parse_line_v(obj, line, &curr);
	else if (is_type(line, "vt", &curr))
		success = parse_line_vt(obj, line, &curr);
	else if (is_type(line, "vn", &curr))
		success = parse_line_vn(obj, line, &curr);
	else if (is_type(line, "f", &curr))
		success = parse_line_f(obj, line, &curr);
	else
		return (true);
	if (!success)
		return (false);
	skip_whitespace(line, &curr);
	if (line[curr] != '\0')
		set_error(ft_strjoin("Line contains more data than expected: ", line),
			true);
	return (line[curr] == '\0');
}

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
		if (out == -1 || !parse_line(obj, line))
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
