/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_line.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 16:21:00 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/04 13:19:21 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_LINE_H
# define MINI_RT_PARSE_LINE_H

# include "mini_rt_parse_utils.h"
# include "libft.h"

bool	parse_resolution(t_scene_parse_data *parse_data,
	t_scene *scene, char *line, int *curr);
bool	parse_ambiant(t_scene_parse_data *parse_data,
	t_scene *scene, char *line, int *curr);
bool	parse_anti_aliasing(t_scene_parse_data *parse_data,
	t_scene *scene, char *line, int *curr);

bool	parse_camera(t_scene *scene, char *line, int *curr);
bool	parse_light(t_scene *scene, char *line, int *curr);
bool	parse_sphere(t_scene *scene, char *line, int *curr);
bool	parse_plane(t_scene *scene, char *line, int *curr);
bool	parse_square(t_scene *scene, char *line, int *curr);
bool	parse_cylinder(t_scene *scene, char *line, int *curr);
bool	parse_triangle(t_scene *scene, char *line, int *curr);
bool	parse_cube(t_scene *scene, char *line, int *curr);

#endif
