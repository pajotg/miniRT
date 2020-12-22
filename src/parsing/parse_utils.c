/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:11:07 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 16:17:05 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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