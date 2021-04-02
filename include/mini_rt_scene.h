/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_scene.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:12:15 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/02 16:08:03 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_SCENE_H
# define MINI_RT_SCENE_H

# include "ft_list.h"
# include "mini_rt_color.h"
# include "mini_rt_bvh.h"

typedef struct s_scene_parse_data
{
	bool	has_resolution;
	bool	has_ambiant;
}				t_scene_parse_data;

typedef struct s_resolution
{
	int	width;
	int	height;
}				t_resolution;

typedef struct s_scene
{
	t_resolution	resolution;
	t_color_hdr		ambiant;
	int				current_camera_index;
	int				noise_reduction;
	bool			use_gi;
	t_list			samples_per_pixel;
	t_list			cameras;
	t_list			objects;
	t_list			lights;
	t_list			directional_lights;
	t_bvh			*bvh;
	char			*scene_path;
}				t_scene;

void			free_scene(t_scene *scene);

bool	parse_object(t_scene_parse_data *parse_data, t_scene *scene, char *line,
					int *curr);
bool	parse_line(t_scene_parse_data *parse_data, t_scene *scene, char *line);

t_scene			*parse_scene_file(char *path, int fd);
t_scene			*parse_scene(char *path);

#endif
