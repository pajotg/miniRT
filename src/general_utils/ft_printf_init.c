/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_init.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 11:37:47 by jasper        #+#    #+#                 */
/*   Updated: 2020/12/25 11:57:55 by jasper        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "mini_rt_math_utils.h"

// Well, my printf gives me the option to add conversions
// So i might as well use it, but i dont have conversions for floating points...
// So........ just use the real printf! YEAH! LAZYNESS
// I did add ft_printf floating point printing to my TODO list
static void	handle_conversion_v(t_format *format, t_print_state *state)
{
	t_vec3 vec3 = va_arg(state->lst, t_vec3);
	state->printed += fprintf(stderr, "%.2f %.2f %.2f", vec3.x, vec3.y, vec3.z);
	(void)format;
}

static void	handle_conversion_q(t_format *format, t_print_state *state)
{
	t_quaternion quat = va_arg(state->lst, t_quaternion);
	state->printed += fprintf(stderr, "%.2f %.2f %.2f %.2f", quat.r, quat.i, quat.j, quat.k);
	(void)format;
}

static void	handle_conversion_t(t_format *format, t_print_state *state)
{
	t_transform trans = va_arg(state->lst, t_transform);
	state->printed += fprintf(stderr, "%.2f %.2f %.2f (%.2f %.2f %.2f %.2f)",
		trans.position.x, trans.position.y, trans.position.z,
		trans.rotation.r, trans.rotation.i, trans.rotation.j, trans.rotation.k
	);
	(void)format;
}

void init_ft_printf(void)
{
	set_conversion_handle('v', handle_conversion_v);
	set_conversion_handle('q', handle_conversion_q);
	set_conversion_handle('t', handle_conversion_t);
}