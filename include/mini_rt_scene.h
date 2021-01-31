/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_scene.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:12:15 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/31 13:04:13 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_SCENE_H
# define MINI_RT_SCENE_H

# include "ft_list.h"
# include "mini_rt_color.h"

typedef struct	s_scene_parse_data
{
	bool has_resolution;
	bool has_ambiant;
}				t_scene_parse_data;

typedef struct	s_resolution
{
	int width;
	int height;
}				t_resolution;

typedef struct	s_scene
{
	t_resolution	resolution;
	t_color_hdr		ambiant;
	int				current_camera_index;
	int				noise_reduction;
	t_list			samples_per_pixel;
	t_list			cameras;
	t_list			objects;
	t_list			lights;
	t_list			directional_lights;
}				t_scene;

void			free_scene(t_scene *scene);
t_scene			*parse_scene_file(int fd);
t_scene			*parse_scene(char *path);

#endif
