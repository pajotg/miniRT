/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_init.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 11:57:46 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/11 22:02:23 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"
#include <stdlib.h>
#include "libft.h"
//#include <stdio.h>

bool	list_init(t_list *list, size_t data_size)
{
	list->data = malloc(data_size * 2);
	if (!list->data)
		return (false);
	list->data_size = data_size;
	list->capacity = 2;
	list->count = 0;
	return (true);
}

t_list	*list_new(size_t data_size)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	if (!list_init(list, data_size))
	{
		free(list);
		return (NULL);
	}
	return (list);
}

void	list_un_init(t_list *list, const t_free_values free_values)
{
	size_t	i;

	if (free_values)
	{
		i = 0;
		while (i < list->count)
		{
			(*free_values)(list->data + list->data_size * i);
			i++;
		}
	}
	free(list->data);
}

void	list_free(t_list *list, const t_free_values free_values)
{
	list_un_init(list, free_values);
	free(list);
}

bool	list_set_capacity(t_list *list, size_t capacity)
{
	void	*new_data;

	if (list->count > capacity)
		return (false);
	new_data = malloc(list->data_size * capacity);
	if (!new_data)
		return (false);
	ft_memcpy(new_data, list->data, list->data_size * list->count);
	free(list->data);
	list->data = new_data;
	list->capacity = capacity;
	return (true);
}
