/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_scene_line.h                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:14:02 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:58:18 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_SCENE_LINE_H
# define MINI_RT_PARSE_SCENE_LINE_H

# include "mini_rt_scene.h"

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
bool	parse_directional_light(t_scene *scene, char *line, int *curr);

#endif
