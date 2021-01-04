/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_args.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/04 12:59:25 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/04 13:19:08 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_parse_utils.h"
#include "ft_error.h"
#include "libft.h"
#include <stdlib.h>

t_args* parse_args(int argc, char **argv)
{
	if (argc <= 1)
	{
		set_error("Usage: miniRT [file] {args}", false);
		return (NULL);
	}
	t_args* data = malloc(sizeof(t_args));
	if (data == NULL)
	{
		set_error("Malloc failed for parse args!", false);
		return NULL;
	}

	data->map_file = argv[1];
	if (ft_strlen(data->map_file) < 4 || ft_strncmp(data->map_file + ft_strlen(data->map_file) - 3,".rt", 3) != 0 || (data->map_file[ft_strlen(data->map_file)-4] == '/'))	// Checks for: strlen >= 4, ends with .rt, and character before . != /
	{
		set_error(ft_strjoin("File does not end with \".rt\", got: ", data->map_file), true);
		free(data);
		return NULL;
	}

	data->save = false;
	data->save_on_exit = false;
	data->no_res_cap = false;
	int i = 2;
	while (i < argc)
	{
		char* arg = argv[i];
		if (ft_strncmp(arg, "--save", 7) == 0 && data->save == false)
			data->save = true;
		else if (ft_strncmp(arg, "--no-res-cap",13) == 0 && data->no_res_cap == false)
			data->no_res_cap = true;
		else if (ft_strncmp(arg, "--save-on-exit",15) == 0 && data->save_on_exit == false)
			data->save_on_exit = true;
		else
		{
			set_error(ft_strjoin("Argument not recognized: ", arg), true);
			free(data);
			return NULL;
		}
		i++;
	}
	if (data->save && data->save_on_exit)
	{
		set_error("Both save and save-on-exit arguments where specified!", false);
		free(data);
		return NULL;
	}

	return data;
}