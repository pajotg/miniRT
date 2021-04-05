/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_frame.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/30 14:20:16 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 14:20:28 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_renderer.h"

bool	is_first_frame(t_pixel_renderer *renderer)
{
	return (renderer->frame_num <= 1);
}

int	get_aa_frame(t_pixel_renderer *renderer, t_scene *scene)
{
	if (renderer->frame_num >= 2 && (size_t)(renderer->frame_num - 2) < scene->
		samples_per_pixel.count)
		return (renderer->frame_num - 2);
	return (-1);
}
