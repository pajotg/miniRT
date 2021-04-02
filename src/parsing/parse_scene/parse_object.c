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

static bool	is_object(char *line, char *object, int *curr)
{
	int	i;

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

bool	parse_object_stupid_norm(t_scene *scene,
	char *line, int *curr)
{
	if (is_object(line, "cy", curr))
		return (scene_parse_cylinder(scene, line, curr));
	else if (is_object(line, "tr", curr))
		return (scene_parse_triangle(scene, line, curr));
	else if (is_object(line, "cu", curr))
		return (scene_parse_cube(scene, line, curr));
	else if (is_object(line, "dl", curr))
		return (scene_parse_directional_light(scene, line, curr));
	else if (is_object(line, "obj", curr))
		return (scene_parse_obj(scene, line, curr));
	return (false);
}

bool	parse_object(t_scene_parse_data *parse_data, t_scene *scene, char *line,
	int *curr)
{
	if (is_object(line, "R", curr))
		return (scene_parse_resolution(parse_data, scene, line, curr));
	else if (is_object(line, "A", curr))
		return (scene_parse_ambiant(parse_data, scene, line, curr));
	else if (is_object(line, "AA", curr))
		return (scene_parse_anti_aliasing(scene, line, curr));
	else if (is_object(line, "NR", curr))
		return (scene_parse_noise_reduction(scene, line, curr));
	else if (is_object(line, "GI", curr))
		return (scene_parse_gi(scene, line, curr));
	else if (is_object(line, "c", curr))
		return (scene_parse_camera(scene, line, curr));
	else if (is_object(line, "l", curr))
		return (scene_parse_light(scene, line, curr));
	else if (is_object(line, "sp", curr))
		return (scene_parse_sphere(scene, line, curr));
	else if (is_object(line, "pl", curr))
		return (scene_parse_plane(scene, line, curr));
	else if (is_object(line, "sq", curr))
		return (scene_parse_square(scene, line, curr));
	else
		return (parse_object_stupid_norm(scene, line, curr));
	set_error(ft_strjoin("Unknown configuration: ", line), true);
	return (false);
}
