/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 14:25:34 by jsimonis      #+#    #+#                 */
/*   Updated: 2020/11/28 15:42:59 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
**	Using a linked list with both a next and previous
**	we keep track of what buffer is for what fd
**	Since we also have previous clearing out a single element is easy
*/

static t_gnl_ll	**get_gnl_ll(void)
{
	static t_gnl_ll	*list = NULL;

	return (&list);
}

t_fd_data		*get_gnl_data(int fd)
{
	t_gnl_ll	**start;
	t_gnl_ll	*current;

	start = get_gnl_ll();
	current = *start;
	while (current != NULL)
	{
		if (current->fd == fd)
			return (&current->data);
		current = current->next;
	}
	current = malloc(sizeof(t_gnl_ll));
	if (current == NULL)
		return (NULL);
	current->data.buffer = NULL;
	current->fd = fd;
	current->next = *start;
	current->previous = NULL;
	if (*start)
		(*start)->previous = current;
	*start = current;
	return (&current->data);
}

bool			clear_gnl_data(int fd)
{
	t_gnl_ll	**start;
	t_gnl_ll	*current;

	start = get_gnl_ll();
	current = *start;
	while (current != NULL)
	{
		if (current->fd == fd)
		{
			if (current == *start)
				*start = current->next;
			if (current->next)
				current->next->previous = current->previous;
			if (current->previous)
				current->previous->next = current->next;
			free(current->data.buffer);
			free(current);
			return (true);
		}
		current = current->next;
	}
	return (false);
}

void			mem_cpy(char *dst, char *src, size_t len)
{
	size_t curr;

	curr = 0;
	while (curr < len)
	{
		dst[curr] = src[curr];
		curr++;
	}
}

/*
**	Can optimize the memcpy to only copy from buff_inx to buff_end
**		instead of from 0 to buff_end
**		but every time increase_buffer_size is called buff_index is zero
*/

bool			increase_buffer_size(t_fd_data *data)
{
	char	*new;
	size_t	size;

	size = data->buff_size * 2;
	new = malloc(size);
	if (new == NULL)
		return (false);
	mem_cpy(new, data->buffer, data->buff_end);
	free(data->buffer);
	data->buffer = new;
	data->buff_size = size;
	return (true);
}
