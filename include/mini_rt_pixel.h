/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_rt_pixel.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 17:21:58 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:11:52 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_PIXEL_H
# define MINI_RT_PIXEL_H

typedef struct	s_pixel_data
{
	t_color_hdr		color;
	unsigned int	num_samples;
}				t_pixel_data;

t_color_hdr		convert_to_hdr(t_pixel_data *pixel);

#endif
