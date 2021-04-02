/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_obj.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/02 17:06:00 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_OBJ_H
# define MINI_RT_PARSE_OBJ_H

# include "ft_vec3.h"
# include "ft_vec2.h"
# include "ft_list.h"

# include "mini_rt_scene.h"
# include "ft_shared_ptr.h"
# include "mini_rt_object.h"

/*
**	Thats alot of info for a face!
*/

typedef struct s_face_vertex
{
	int	vertex_index;
	int	texture_index;
	int	normal_index;
}				t_face_vertex;

typedef struct s_face
{
	t_face_vertex	a;
	t_face_vertex	b;
	t_face_vertex	c;
}				t_face;

typedef struct s_obj
{
	t_list	vertexes;
	t_list	normals;
	t_list	uvs;
	t_list	faces;
}				t_obj;

void	free_obj(t_obj *obj);

bool	parse_line_f(t_obj *obj, char *line, int *curr);
bool	obj_parse_line(t_obj *obj, char *line);

bool	insert_obj_into_scene(t_scene *scene, t_obj *obj,
	const t_transform *transform, t_shared_pt *material);

t_obj	*parse_obj_file(int fd);
t_obj	*parse_obj(char *path);

#endif
