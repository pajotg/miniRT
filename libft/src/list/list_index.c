#include "ft_list.h"

void	*list_index_unchecked(const t_list *list, size_t index)
{
	return (list->data + index * list->data_size);
}

void	*list_index(const t_list *list, size_t index)
{
	if (index >= list->count)
		return (NULL);
	return (list->data + index * list->data_size);
}
