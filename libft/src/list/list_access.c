/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_access.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/03 12:28:48 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/03/20 17:26:30 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_list.h"

bool	list_push(t_list *list, const void *value)
{
	if (list->count == list->capacity
		&& !list_set_capacity(list, list->capacity * 2))
		return (false);
	ft_memcpy(list->data + list->data_size * list->count,
		value, list->data_size);
	list->count++;
	return (true);
}

void	*list_pop(t_list *list)
{
	void	*ret;

	if (list->count == 0)
		return (NULL);
	ret = list_index(list, list->count - 1);
	list->count--;
	return (ret);
}

bool	list_insert(t_list *list, size_t index, const void *value)
{
	void	*location;

	if (index > list->count)
		return (false);
	if (list->count == list->capacity
		&& !list_set_capacity(list, list->capacity * 2))
		return (false);
	location = list->data + index * list->data_size;
	ft_memmove(location + list->data_size, location,
		list->data_size * (list->count - index));
	ft_memcpy(location, value, list->data_size);
	list->count++;
	return (true);
}

bool	list_remove(t_list *list, size_t index)
{
	void	*location;

	if (index >= list->count)
		return (false);
	location = list->data + index * list->data_size;
	ft_memmove(location, location + list->data_size,
		list->data_size * (list->count - index - 1));
	list->count--;
	return (true);
}
