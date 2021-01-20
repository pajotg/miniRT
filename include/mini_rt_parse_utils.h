/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_utils.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:19 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/20 14:30:39 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_UTILS_H
# define MINI_RT_PARSE_UTILS_H

# include "list.h"
# include "mini_rt_math_utils.h"
# include "ft_vec2.h"

typedef struct	s_args
{
	char *map_file;
	bool save;
	bool save_on_exit;
	bool no_res_cap;
}				t_args;

typedef struct	s_resolution
{
	int width;
	int height;
}				t_resolution;

typedef struct	s_scene_parse_data
{
	bool has_resolution;
	bool has_ambiant;
	bool has_anti_aliasing;
}				t_scene_parse_data;

typedef struct	s_scene
{
	t_resolution	resolution;
	t_color_hdr		ambiant;
	int				current_camera_index;
	int				samples_per_pixel;
	t_list			cameras;
	t_list			objects;
	t_list			lights;
	t_list			directional_lights;
}				t_scene;

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

void			free_scene(t_scene *scene);
t_scene			*parse_scene_file(int fd);
t_args			*parse_args(int argc, char **argv);

bool			skip_char(char *str, int *current, char chr);
void			skip_whitespace(char *str, int *current);
bool			read_int(char *str, int *current, int *value);
bool			read_float(char *str, int *current, float *value);

bool			read_vec3(char *str, int *current, t_vec3 *vec3);
bool			read_vec3_unit(char *str, int *current, t_vec3 *vec3);
bool			read_transform(char *str, int *current, t_transform *transform);
bool			read_color(char *str, int *current, bool has_ratio,
	t_color_hdr *color);

#endif
