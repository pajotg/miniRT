/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_obj.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:22 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 17:59:03 by jsimonis      ########   odam.nl         */
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

typedef struct	s_face
{
	t_vec3 a_pos;
	t_vec3 b_pos;
	t_vec3 c_pos;

	t_vec2 a_uv;
	t_vec2 b_uv;
	t_vec2 c_uv;

	t_vec3 a_normal;
	t_vec3 b_normal;
	t_vec3 c_normal;
}				t_face;

typedef struct	s_obj
{
	t_list	faces;
}				t_obj;

#endif
