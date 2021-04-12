/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/03 12:05:51 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/07 14:29:52 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIST_H
# define FT_LIST_H

# include <stdbool.h>
# include <stddef.h>

/*
**	I did some stuff in c# before codam, and this is what a list was
**	I think its called a vector in c++, oh well, dont matter
**	a list is definently NOT a linked list!
*/

typedef struct s_list
{
	size_t	count;
	size_t	capacity;
	size_t	data_size;
	void	*data;
}				t_list;

typedef void	(*t_free_values)(void *value);

bool			list_init(t_list *list, size_t data_size);
void			list_un_init(t_list *list, const t_free_values free_values);

t_list			*list_new(size_t data_size);
void			list_free(t_list *list, const t_free_values free_values);

bool			list_set_capacity(t_list *list, size_t capacity);

void			*list_index_unchecked(const t_list *list, size_t index);
void			*list_index(const t_list *list, size_t index);

bool			list_push(t_list *list, const void *value);
void			*list_pop(t_list *list);

bool			list_insert(t_list *list, size_t index, const void *value);
bool			list_remove(t_list *list, size_t index);

void			list_clear(t_list *list, const t_free_values free_values);

#endif
