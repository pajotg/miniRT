/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_utils.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 11:26:48 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 15:40:37 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_UTILS_H
# define MINI_RT_UTILS_H

#include <stdbool.h>
#include <stddef.h>

/*
**	This is actually called a "List"
**	But libft also has a list defintion, but its not a list, its a linked list!
**	Darn you libft mandatory naming!
**	So i called it "Dynamic Array", aka: darray.. stupid mandatory naming..
*/

typedef struct s_darray
{
	size_t count;
	size_t capacity;
	size_t data_size;
	void* data;
}				t_darray;

typedef void (*t_free_values)(void *value);

char* get_last_error();
void set_error(char* error_msg, bool should_free);
void clear_error();

bool darray_init(t_darray *darr, size_t data_size);
t_darray *darray_new(size_t data_size);
void darray_un_init(t_darray *darr, t_free_values free_values);
bool darray_push(t_darray *darr, void *value);

#endif