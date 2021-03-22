/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_parse_scene_line.h                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:14:02 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/05 12:18:50 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PARSE_SCENE_LINE_H
# define MINI_RT_PARSE_SCENE_LINE_H

# include "mini_rt_scene.h"

bool	scene_parse_resolution(t_scene_parse_data *parse_data,
			t_scene *scene, char *line, int *curr);
bool	scene_parse_ambiant(t_scene_parse_data *parse_data,
			t_scene *scene, char *line, int *curr);
bool	scene_parse_anti_aliasing(t_scene *scene, char *line, int *curr);
bool	scene_parse_noise_reduction(t_scene *scene, char *line, int *curr);
bool	scene_parse_gi(t_scene *scene, char *line, int *curr);

bool	scene_parse_camera(t_scene *scene, char *line, int *curr);
bool	scene_parse_light(t_scene *scene, char *line, int *curr);
bool	scene_parse_sphere(t_scene *scene, char *line, int *curr);
bool	scene_parse_plane(t_scene *scene, char *line, int *curr);
bool	scene_parse_square(t_scene *scene, char *line, int *curr);
bool	scene_parse_cylinder(t_scene *scene, char *line, int *curr);
bool	scene_parse_triangle(t_scene *scene, char *line, int *curr);
bool	scene_parse_cube(t_scene *scene, char *line, int *curr);
bool	scene_parse_directional_light(t_scene *scene, char *line, int *curr);
bool	scene_parse_obj(t_scene *scene, char *line, int *curr);

#endif
