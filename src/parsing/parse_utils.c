/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:11:07 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/26 18:26:53 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "ft_parse.h"
#include "mini_rt_object.h"

bool read_transform(char* str, int* current, t_transform *transform)
{
	if (!read_vec3(str, current, &transform->position))
		return false;
	skip_whitespace(str, current);
	t_vec3 forward;
	if (!read_vec3_unit(str, current, &forward))
		return false;
	float dot = vec3_dot(&forward, vec3_up());
	if ( dot > 0.999 || dot < -0.999)
		quaternion_from_forward_up(&transform->rotation, &forward, vec3_right());
	else
		quaternion_from_forward_up(&transform->rotation, &forward, vec3_up());
	return true;
}

bool read_color(char* str, int* current, bool has_ratio, t_color_hdr *color)
{
	int read;
	float ratio;
	if (has_ratio)
	{
		if (!read_float(str, current, &ratio))
			return false;
		if (ratio < 0 || ratio > 1)
			return false;
		ratio /= 255;
		skip_whitespace(str, current);
	}
	else
		ratio = 1 / 255.0;

	if (!read_int(str, current, &read))
		return false;
	color->r = read * ratio;
	if (!skip_char(str, current, ',') || read < 0 || read > 255)
		return false;

	if (!read_int(str, current, &read))
		return false;
	color->g = read * ratio;
	if (!skip_char(str, current, ',') || read < 0 || read > 255)
		return false;

	if (!read_int(str, current, &read))
		return false;
	color->b = read * ratio;
	if (read < 0 || read > 255)
		return false;

	return true;
}