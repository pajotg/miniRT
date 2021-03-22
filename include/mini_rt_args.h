/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_args.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:12:23 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 17:55:27 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_ARGS_H
# define MINI_RT_ARGS_H

typedef struct s_args
{
	char	*map_file;
	bool	save;
	bool	save_on_exit;
	bool	no_res_cap;
}				t_args;

t_args			*parse_args(int argc, char **argv);

#endif
