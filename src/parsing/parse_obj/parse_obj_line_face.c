#include "libft.h"
#include "mini_rt_parse_utils.h"
#include "mini_rt_parse_obj.h"
#include "ft_get_next_line.h"
#include "ft_parse_utils.h"
#include "ft_vec2.h"
#include <stdbool.h>
#include <stdlib.h>
#include "ft_error.h"

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

bool	parse_line_f(t_obj *obj, char *line, int *curr)
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
