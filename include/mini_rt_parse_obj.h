/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_obj.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/02/05 13:17:23 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_OBJ_H
# define MINI_RT_PARSE_OBJ_H

# include "ft_vec3.h"
# include "ft_vec2.h"
# include "ft_list.h"

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

t_obj	*parse_obj_file(int fd);
t_obj	*parse_obj(char *path);

#endif
