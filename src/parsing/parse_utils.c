/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:11:07 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 13:47:29 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "mini_rt_utils.h"
#include "mini_rt_math_utils.h"

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

int read_int(char* str, int* current)
{
	int value = 0;

	// Handle negatives
	bool IsNegative = str[*current] == '-';
	if (IsNegative)
		(*current)++;

	// Basic atoi
	while (ft_isdigit(str[*current]))
	{
		value = value * 10 + str[*current] - '0';
		if (value < 0)
		{
			set_error("Value out of range!", false);
			if (IsNegative)
				return -2147483648;
			return 2147483647;
		}
		(*current)++;
	}

	if (IsNegative)
		return (-value);
	return (value);
}

float read_float(char* str, int* current)
{
	float value = 0;

	// Handle negatives
	bool IsNegative = str[*current] == '-';
	if (IsNegative)
		(*current)++;

	// Basic atoi
	while (ft_isdigit(str[*current]))
	{
		value = value * 10 + str[*current] - '0';
		(*current)++;
	}

	// Handle decimal point
	if (str[*current] == '.')
	{
		(*current)++;
		float Worth = 0.1f;
		while (ft_isdigit(str[*current]))
		{
			value += (str[*current] - '0') * Worth;
			Worth /= 10;
			(*current)++;
		}
	}

	if (IsNegative)
		return (-value);
	return (value);
}

bool read_vec3(char* str, int* current, t_vec3 *vec3)
{
	vec3->x = read_float(str, current);
	if (!skip_char(str, current, ','))
		return false;
	vec3->y = read_float(str, current);
	if (!skip_char(str, current, ','))
		return false;
	vec3->z = read_float(str, current);
	return true;
}

bool read_transform(char* str, int* current, t_transform *transform)
{
	if (!read_vec3(str, current, &transform->position))
		return false;
	skip_whitespace(str, current);
	t_vec3 forward;
	if (!read_vec3(str, current, &forward))
		return false;
	transform->rotation = quaternion_from_forward_up(forward, vec3_new(0, 1, 0));
	return true;
}

bool read_color(char* str, int* current, t_color_hdr *color)
{
	int read;
	float ratio = read_float(str, current);
	skip_whitespace(str, current);

	read = read_int(str, current);
	color->r = read * ratio / 255;
	if (!skip_char(str, current, ',') || read < 0 || read > 255)
		return false;

	read = read_int(str, current);
	color->g = read * ratio / 255;
	if (!skip_char(str, current, ',') || read < 0 || read > 255)
		return false;

	read = read_int(str, current);
	color->b = read * ratio / 255;
	if (read < 0 || read > 255)
		return false;

	return true;
}