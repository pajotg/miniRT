/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_parse_utils_skip.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:58:25 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 22:01:19 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//#include <stdbool.h>
#include "libft.h"

bool	skip_char(const char *str, int *current, char chr)
{
	if (str[*current] == chr)
	{
		(*current)++;
		return (true);
	}
	return (false);
}

void	skip_whitespace(const char *str, int *current)
{
	while (ft_isspace(str[*current]))
		(*current)++;
}
