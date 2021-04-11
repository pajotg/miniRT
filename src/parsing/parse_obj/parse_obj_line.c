#include "libft.h"
//#include "mini_rt_parse_utils.h"
#include "mini_rt_parse_obj.h"
//#include "ft_get_next_line.h"
#include "ft_parse_utils.h"
//#include "ft_vec2.h"
//#include <stdbool.h>
//#include <stdlib.h>
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

//return (false);
//set_error(ft_strjoin("Unknown line type: ", line), true);
// type: vp, parameter space vertices, o
// face
// vertex normal
// texture coordinate
// vertex

bool	obj_parse_line(t_obj *obj, char *line)
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
