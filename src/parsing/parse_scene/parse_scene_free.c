/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_scene_free.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 19:27:40 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/02 15:53:12 by jsimonis      ########   odam.nl         */
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

void	un_init_object(t_object *object)
{
	free(object->object_data);
	shared_pt_release_and_free(object->material);
}

void	free_scene(t_scene *scene)
{
	list_un_init(&(scene->cameras), NULL);
	list_un_init(&(scene->objects), (t_free_values)un_init_object);
	list_un_init(&(scene->lights), NULL);
	list_un_init(&(scene->directional_lights), NULL);
	list_un_init(&(scene->samples_per_pixel), NULL);
	if (scene->bvh)
		bvh_free(scene->bvh, NULL);
	free(scene);
}
