/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:11:07 by jasper        #+#    #+#                 */
/*   Updated: 2021/01/15 14:46:32 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "mini_rt_math_utils.h"
#include "ft_error.h"

#include <stdio.h>

bool skip_char(char* str, int* current, char chr)
{
	if (str[*current] == chr)
	{
		(*current)++;
		return (true);
	}
	return (false);
}

void skip_whitespace(char* str, int* current)
{
	while (ft_isspace(str[*current]))
		(*current)++;
}

bool read_int(char* str, int* current, int* value)
{
	int start = *current;
	*value = 0;

	// Handle negatives
	bool IsNegative = skip_char(str, current, '-');

	// Basic atoi
	while (ft_isdigit(str[*current]))
	{
		*value = (*value) * 10 + str[*current] - '0';
		if (*value < 0)
		{
			set_error("Value out of range!", false);
			if (IsNegative)
				*value = -2147483648;
			*value = 2147483647;
			return false;
		}
		(*current)++;
	}

	if (IsNegative)
		*value = -*value;
	return (start != *current);
}

bool read_float(char* str, int* current, float* value)
{
	int start = *current;
	*value = 0;

	// Handle negatives
	bool IsNegative = skip_char(str, current, '-');

	// Basic atoi
	while (ft_isdigit(str[*current]))
	{
		*value = *value * 10 + str[*current] - '0';
		(*current)++;
	}

	// Handle decimal point
	if (skip_char(str, current, '.'))
	{
		float Worth = 0.1f;
		while (ft_isdigit(str[*current]))
		{
			*value += (str[*current] - '0') * Worth;
			Worth /= 10;
			(*current)++;
		}
	}

	if (IsNegative)
		*value = -*value;
	return (start != *current);
}

bool read_vec3(char* str, int* current, t_vec3 *vec3)
{
	if (!read_float(str, current, &vec3->x) || !skip_char(str, current, ','))
		return false;
	if (!read_float(str, current, &vec3->y) || !skip_char(str, current, ','))
		return false;
	if (!read_float(str, current, &vec3->z))
		return false;
	return true;
}

bool read_vec3_unit(char* str, int* current, t_vec3 *vec3)
{
	if (!read_vec3(str, current, vec3))
		return false;
	float magnitude_sqr = vec3_magnitude_sqr(vec3);
	if (magnitude_sqr < 0.001)
		return false;
	vec3_normalize(vec3, vec3);
	return true;
}

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