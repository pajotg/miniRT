/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_pixel.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:21:58 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/12 14:40:23 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PIXEL_H
# define MINI_RT_PIXEL_H

# include "mini_rt_color.h"
# include "mini_rt_hilbert_randomizer.h"

typedef struct s_pixel_data
{
	t_color_hdr		color;
	unsigned int	num_samples;
}				t_pixel_data;

typedef struct s_temp_pixel_data
{
	t_pixel_data			pixel_data;
	float					aa_difference;
	t_hilbert_randomizer	randomizer;
}				t_temp_pixel_data;

t_color_hdr		convert_to_hdr(t_pixel_data *pixel);
float			get_difference(t_color_hdr *col_a, t_color_hdr *col_b);;
#endif
