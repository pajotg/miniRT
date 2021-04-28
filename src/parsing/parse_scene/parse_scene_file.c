//#include "mini_rt_scene.h"
//#include "mini_rt_parse_scene_line.h"
#include "ft_error.h"
#include "libft.h"
#include "ft_get_next_line.h"
#include "mini_rt_object.h"
//#include "ft_parse_utils.h"
#include <stdlib.h>
//#include "mini_rt_material_data.h"
//#include "ft_shared_ptr.h"

/*
**	doing init_success = list_init(&list, sizeof(t_camera)) && init_success;
**	because otherwise if a malloc fails, init_success gets set to false,
**		and then the other list_init dont get called, but we DO call list_un_init
**		causing it to free un-initialized memory, it has not caused a crash for me
**		because un-initialized memory is zero'd on my system
*/

static t_scene	*init_scene_stupid_norm(void)
{
	bool	init_success;
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (scene == NULL)
	{
		set_error("Malloc failed in parse_scene_file!", false);
		return (NULL);
	}
	init_success = true;
	init_success = list_init(&scene->cameras, sizeof(t_camera)) && init_success;
	init_success = list_init(&scene->objects, sizeof(t_object)) && init_success;
	init_success = list_init(&scene->lights, sizeof(t_light)) && init_success;
	init_success = list_init(&scene->directional_lights, sizeof(
				t_directional_light)) && init_success;
	init_success = list_init(&scene->samples_per_pixel, sizeof(int))
		 && init_success;
	if (!init_success)
	{
		free_scene(scene);
		set_error("Malloc failed in list init!", false);
		return (NULL);
	}
	return (scene);
}

static bool	do_ze_parsing_stupid_norm(t_scene_parse_data *parse_data, t_scene *
	scene, int fd)
{
	char	*line;
	int		out;

	while (true)
	{
		out = get_next_line(fd, &line);
		if (out == -1)
		{
			set_error(ft_strjoin("Could not read file: ", get_last_error()),
				true);
			return (false);
		}
		if (!parse_line(parse_data, scene, line))
		{
			free(line);
			return (false);
		}
		free(line);
		if (out == 0)
			break ;
	}
	return (true);
}

static bool	is_this_a_valid_scene_stupid_norm(t_scene *scene,
	t_scene_parse_data *parse_data)
{
	if ((!scene->bvh && scene->objects.count > 0) || !parse_data->has_ambiant
		|| !parse_data->has_resolution || scene->cameras.count == 0
		|| (scene->noise_reduction != 0 && scene->samples_per_pixel.count == 0))
	{
		if (!parse_data->has_ambiant)
			set_error("No ambiant in configuration!", false);
		else if (!parse_data->has_resolution)
			set_error("No resolution in configuration!", false);
		else if (scene->cameras.count == 0)
			set_error("No cameras in configuration", false);
		else if (scene->noise_reduction != 0 && scene
			->samples_per_pixel.count == 0)
			set_error("Noise reduction set without AA!", false);
		else if (!scene->bvh)
			set_error("Failed to build BVH!", false);
		else
			set_error("Wait, what even errored?", false);
		free_scene(scene);
		return (false);
	}
	return (true);
}

t_scene	*parse_scene_file(char *path, int fd)
{
	t_scene_parse_data	parse_data;
	t_scene				*scene;

	scene = init_scene_stupid_norm();
	if (!scene)
		return (NULL);
	scene->current_camera_index = 0;
	scene->noise_reduction = 0;
	scene->use_gi = false;
	scene->scene_path = path;
	scene->bvh = NULL;
	parse_data.has_ambiant = false;
	parse_data.has_resolution = false;
	if (!do_ze_parsing_stupid_norm(&parse_data, scene, fd))
	{
		clear_gnl_data(fd);
		free_scene(scene);
		return (NULL);
	}
	scene->bvh = bvh_build(&scene->objects);
	if (!is_this_a_valid_scene_stupid_norm(scene, &parse_data))
		return (NULL);
	return (scene);
}
