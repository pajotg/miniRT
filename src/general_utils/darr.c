/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   darr.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/23 11:57:46 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/23 14:22:06 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_utils.h"
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

bool darray_init(t_darray *darr, size_t data_size)
{
	darr->data = malloc(data_size * 16);
	if (!darr->data)
		return false;
	darr->data_size = data_size;
	darr->capacity = 16;
	darr->count = 0;
	return true;
}

t_darray *darray_new(size_t data_size)
{
	t_darray *darr;

	darr = malloc(sizeof(t_darray));
	if (!darr)
		return (NULL);
	if (!darray_init(darr, data_size))
	{
		free(darr);
		return NULL;
	}
	return darr;
}


void darray_un_init(t_darray *darr)
{
	free(darr->data);
}

bool darray_push(t_darray *darr, void *value)
{
	void *data;
	if (darr->count == darr->capacity)
	{
		data = malloc(darr->data_size * darr->capacity * 2);
		if (!value)
			return (false);
		ft_memcpy(data, darr->data, darr->data_size * darr->count);
		free(darr->data);
		darr->data = data;
		darr->capacity *= 2;
	}
	else
		data = darr->data;
	ft_memcpy(data + darr->data_size * darr->count, value, darr->data_size);
	darr->count++;
	return (true);
}

/*
** Functions that can be added, but i have no need for yet:
** darray_pop
** darray_insert
** darray_remove
*/