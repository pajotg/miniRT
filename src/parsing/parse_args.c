/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_args.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/04 12:59:25 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/05/22 18:28:58 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "libft.h"
#include <stdlib.h>
#include "mini_rt_args.h"

static bool	parse_flags(t_args *data, int argc, char **argv)
{
	int		i;

	data->save = false;
	data->save_on_exit = false;
	data->no_res_cap = false;
	i = 2;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "--save", 7) == 0 && data->save == false)
			data->save = true;
		else if (ft_strncmp(argv[i], "--no-res-cap", 13) == 0 && data
			->no_res_cap == false)
			data->no_res_cap = true;
		else if (ft_strncmp(argv[i], "--save-on-exit", 15) == 0 && data
			->save_on_exit == false)
			data->save_on_exit = true;
		else
		{
			set_error(ft_strjoin("Argument not recognized: ", argv[i]), true);
			free(data);
			return (false);
		}
		i++;
	}
	return (true);
}

static t_args	*init(int argc)
{
	t_args	*data;

	if (argc <= 1)
	{
		set_error("Usage: miniRT [file] {args}", false);
		return (NULL);
	}
	data = malloc(sizeof(t_args));
	if (data == NULL)
	{
		set_error("Malloc failed for parse args!", false);
		return (NULL);
	}
	return (data);
}

// Checks for: strlen >= 4, ends with .rt, and character before . != /

static bool	validate(t_args *data)
{
	if (data->save && data->save_on_exit)
	{
		set_error("Both save and save-on-exit arguments where specified!",
			false);
		return (false);
	}
	//if (data->save)
	//	data->no_res_cap = true;
	return (true);
}

t_args	*parse_args(int argc, char **argv)
{
	t_args	*data;

	data = init(argc);
	if (!data)
		return (NULL);
	data->map_file = argv[1];
	if (ft_strlen(data->map_file) < 4 || ft_strncmp(data->map_file + ft_strlen(
				data->map_file) - 3, ".rt", 3) != 0 || (data
			->map_file[ft_strlen(data->map_file) - 4] == '/'))
	{
		set_error(ft_strjoin("File does not end with \".rt\", got: ", data
				->map_file), true);
		free(data);
		return (NULL);
	}
	if (!parse_flags(data, argc, argv))
		return (NULL);
	if (!validate(data))
	{
		free(data);
		return (NULL);
	}
	return (data);
}
