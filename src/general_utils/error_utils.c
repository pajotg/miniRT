/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 11:25:25 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 12:36:52 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool* get_should_free()
{
	static bool should_free = false;
	return &should_free;
}

static char** get_error_location()
{
	static char* error_msg = NULL;
	return &error_msg;
}

char* get_last_error()
{
	return *get_error_location();
}

void set_error(char* error_msg, bool should_free)
{
	if (*get_should_free())
		free(*get_error_location());
	if (error_msg == NULL)
		return set_error("malloc failed!", false);
	*get_error_location() = error_msg;
	*get_should_free() = should_free;
}

void clear_error()
{
	if (*get_should_free())
		free(*get_error_location());
	*get_error_location() = NULL;
	*get_should_free() = false;
}