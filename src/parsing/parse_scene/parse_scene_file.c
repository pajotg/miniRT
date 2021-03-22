/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_scene_file.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 19:27:40 by jasper        #+#    #+#                 */
/*   Updated: 2021/02/06 13:32:18 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_scene.h"
#include "mini_rt_parse_scene_line.h"
#include "ft_error.h"
#include "libft.h"
#include "ft_get_next_line.h"
#include "mini_rt_object.h"
#include "ft_parse_utils.h"
#include <stdlib.h>
#include "mini_rt_material_data.h"
#include "ft_shared_ptr.h"

static bool is_object(char *line, char *object, int *curr)
{
	int i;

	i = *curr;
	while (*object)
	{
		if (line[i] != *object)
			return (false);
		object++;
		i++;
	}
	if (!ft_isspace(line[i]) && line[i] != '\0')
		return (false);
	*curr = i;
	return (true);
}

static bool parse_object(t_scene_parse_data *parse_data, t_scene *scene, char *line, int *curr)
{
	if (is_object(line, "R", curr))
		return scene_parse_resolution(parse_data, scene, line, curr);
	else if (is_object(line, "A", curr))
		return scene_parse_ambiant(parse_data, scene, line, curr);
	else if (is_object(line, "AA", curr))
		return scene_parse_anti_aliasing(scene, line, curr);
	else if (is_object(line, "NR", curr))
		return scene_parse_noise_reduction(scene, line, curr);
	else if (is_object(line, "GI", curr))
		return scene_parse_gi(scene, line, curr);
	else if (is_object(line, "c", curr))
		return scene_parse_camera(scene, line, curr);
	else if (is_object(line, "l", curr))
		return scene_parse_light(scene, line, curr);
	else if (is_object(line, "sp", curr))
		return scene_parse_sphere(scene, line, curr);
	else if (is_object(line, "pl", curr))
		return scene_parse_plane(scene, line, curr);
	else if (is_object(line, "sq", curr))
		return scene_parse_square(scene, line, curr);
	else if (is_object(line, "cy", curr))
		return scene_parse_cylinder(scene, line, curr);
	else if (is_object(line, "tr", curr))
		return scene_parse_triangle(scene, line, curr);
	else if (is_object(line, "cu", curr))
		return scene_parse_cube(scene, line, curr);
	else if (is_object(line, "dl", curr))
		return scene_parse_directional_light(scene, line, curr);
	else if (is_object(line, "obj", curr))
		return scene_parse_obj(scene, line, curr);
	set_error(ft_strjoin("Unknown configuration: ", line), true);
	return (false);
}

static bool parse_line(t_scene_parse_data *parse_data, t_scene *scene, char *line)
{
	int curr;

	if (line[0] == '\0' || line[0] == '#')
		return (true);
	curr = 0;
	if (!parse_object(parse_data, scene, line, &curr))
		return (false);

	skip_whitespace(line, &curr);
	if (line[curr] != '\0')
	{
		set_error(ft_strjoin("Line contains more data than expected: ", line), true);
		return false;
	}
	return true;
}

void un_init_object(t_object* object)
{
	free(object->object_data);
	shared_pt_release_and_free(object->material);
}

void free_scene(t_scene* scene)
{
	list_un_init(&(scene->cameras), NULL);
	list_un_init(&(scene->objects), (t_free_values)un_init_object);
	list_un_init(&(scene->lights), NULL);
	list_un_init(&(scene->directional_lights), NULL);
	list_un_init(&(scene->samples_per_pixel), NULL);
	if (scene->bvh)
		bvh_free(scene->bvh);
	free(scene);
}

/*
**	doing init_success = list_init(&list, sizeof(t_camera)) && init_success;
**	because otherwise if a malloc fails, init_success gets set to false,
**		and then the other list_init dont get called, but we DO call list_un_init
**		causing it to free un-initialized memory, it has not caused a crash for me
**		because un-initialized memory is zero'd on my system
*/

t_scene* parse_scene_file(char* path, int fd)
{
	t_scene* scene = malloc(sizeof(t_scene));
	if (scene == NULL)
	{
		set_error("Malloc failed in parse_scene_file!", false);
		return NULL;
	}
	bool init_success = true;
	init_success = list_init(&scene->cameras, sizeof(t_camera)) && init_success;
	init_success = list_init(&scene->objects, sizeof(t_object)) && init_success;
	init_success = list_init(&scene->lights, sizeof(t_light)) && init_success;
	init_success = list_init(&scene->directional_lights, sizeof(t_directional_light)) && init_success;
	init_success = list_init(&scene->samples_per_pixel, sizeof(int)) && init_success;
	if (!init_success)
	{
		free_scene(scene);
		set_error("Malloc failed in list init!", false);
		return NULL;
	}
	char* line;
	scene->current_camera_index = 0;
	scene->noise_reduction = 0;
	scene->use_gi = false;
	scene->scene_path = path;
	scene->bvh = NULL;

	t_scene_parse_data parse_data;
	parse_data.has_ambiant = false;
	parse_data.has_resolution = false;
	while (true)
	{
		//ft_printf("Objects capacity: %lu/%lu\n", scene->objects.count,scene->objects.capacity);
		int out = get_next_line(fd, &line);
		if (out == -1)
		{
			set_error(ft_strjoin("Could not read file: ", get_last_error()), true);
			clear_gnl_data(fd);
			free_scene(scene);
			return NULL;
		}

		if (!parse_line(&parse_data, scene, line))
		{
			clear_gnl_data(fd);
			free(line);
			free_scene(scene);
			return NULL;
		}
		free(line);

		if (out == 0)
			break;
	}
	/*
	if (scene->samples_per_pixel.count == 0)
	{
		const int default_spp = 16;
		if (!list_push(&scene->samples_per_pixel, &default_spp))
		{
			set_error("Failed to add default AA", false);
			free_scene(scene);
			return NULL;
		}
	}
	*/

	scene->bvh = bvh_build(&scene->objects);

	if (!scene->bvh || !parse_data.has_ambiant || !parse_data.has_resolution || scene->cameras.count == 0 || (scene->noise_reduction != 0 && scene->samples_per_pixel.count == 0))
	{
		if (!parse_data.has_ambiant)
			set_error("No ambiant in configuration!", false);
		else if (!parse_data.has_resolution)
			set_error("No resolution in configuration!", false);
		else if (scene->cameras.count == 0)
			set_error("No cameras in configuration", false);
		else if (scene->noise_reduction != 0 && scene->samples_per_pixel.count == 0)
			set_error("Noise reduction set without AA!", false);
		else if (!scene->bvh)
			set_error("Failed to build BVH!", false);
		else
			set_error("Wait, what even errored?", false);
		free_scene(scene);
		return NULL;
	}

	return scene;
}