/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 16:25:22 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/22 19:28:13 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include "mini_rt_math_utils.h"
#include "mini_rt_parse_utils.h"
#include <stdbool.h>

typedef struct	s_args
{
	char* error_msg;
	char* map_file;
	bool save;
}				t_args;

#endif