/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_free.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/28 15:42:51 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/29 15:05:35 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_material.h"
#include <stdlib.h>

void	material_free(t_material *material)
{
	material->material_free_data_func(material->material_data);
	free(material);
}
