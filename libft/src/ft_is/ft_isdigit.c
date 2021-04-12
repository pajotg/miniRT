/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isdigit.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/26 13:27:25 by jsimonis      #+#    #+#                 */
/*   Updated: 2020/10/26 16:22:46 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

bool	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
