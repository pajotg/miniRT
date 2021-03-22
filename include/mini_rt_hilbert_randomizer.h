/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_hilbert_randomizer.h                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/05 10:22:24 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/02/26 10:55:27 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_HILBERT_RANDOMIZER_H
# define MINI_RT_HILBERT_RANDOMIZER_H

# include "ft_vec2.h"

typedef struct s_hilbert_curve
{
	int	recursion;
	int	num_indexes;
	int	size;
}				t_hilbert_curve;

typedef struct s_hilbert_randomizer
{
	t_hilbert_curve	curve;
	int				index;
	int				stage;
}				t_hilbert_randomizer;

void			hilbert_curve_init(t_hilbert_curve *curve, int recursion);
void			hilbert_curve_get(int index,
					t_hilbert_curve *curve, int *o_x, int *o_y);

void			hilbert_randomizer_init(t_hilbert_randomizer *curve);
void			hilbert_randomizer_init_custom(
					t_hilbert_randomizer *curve, int index, int stage,
					int recursion);
t_vec2			hilbert_randomizer_get_next(t_hilbert_randomizer *curve);

#endif
