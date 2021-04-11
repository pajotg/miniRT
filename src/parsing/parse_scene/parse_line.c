#include "mini_rt_scene.h"
//#include "mini_rt_parse_scene_line.h"
#include "ft_error.h"
#include "libft.h"
//#include "ft_get_next_line.h"
//#include "mini_rt_object.h"
#include "ft_parse_utils.h"
//#include <stdlib.h>
//#include "mini_rt_material_data.h"
//#include "ft_shared_ptr.h"

bool	parse_line(t_scene_parse_data *parse_data, t_scene *scene, char *line)
{
	int	curr;

	if (line[0] == '\0' || line[0] == '#')
		return (true);
	curr = 0;
	if (!parse_object(parse_data, scene, line, &curr))
		return (false);
	skip_whitespace(line, &curr);
	if (line[curr] != '\0')
	{
		set_error(ft_strjoin("Line contains more data than expected: ", line),
			true);
		return (false);
	}
	return (true);
}
