/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render_pixel.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 13:59:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/05 13:34:45 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "libft.h"
#include "mini_rt_render_pixel.h"
#include "mini_rt_hooks.h"
#include "mini_rt_image.h"
#include "ft_list.h"
#include <math.h>

// Fast render update for first frame
// for the first frame, dont use random scattering

static void	render_first_pixel(t_mlx_data *data, int x, int y)
{
	t_temp_pixel_data	*pixel_data;
	t_color_hdr			*hdr;
	t_ray				ray;

	pixel_data = &data->renderer.temp_pixels[x + y * data->scene
		->resolution.width];
	hdr = &pixel_data->pixel_data.color;
	pix_to_ray(data, x, y, &ray);
	trace_color(data->scene, &ray, 0, hdr);
	pixel_data->pixel_data.num_samples = 1;
	write_pix(data, x, y, convert_to_hdr(&pixel_data->pixel_data));
}

// AA frame
// Noise reduction frame

static int	calculate_spp(t_mlx_data *data, int x, int y)
{
	float	spp_value;
	int		aa_index;
	int		spp;

	aa_index = get_aa_frame(&data->renderer, data->scene);
	if (aa_index != -1)
	{
		spp_value = get_edgyness(data, x, y) * 15;
		if (spp_value > 1)
			spp_value = 1;
		spp = *(int *)list_index(&data->scene->samples_per_pixel, aa_index)
			 * spp_value;
		if (spp <= 0 && aa_index == 0)
			spp = 1;
	}
	else
	{
		spp_value = (get_noisyness(data, x, y) / data->renderer.avg_noise) - 1;
		if (spp_value < 0)
			spp_value = 0;
		if (spp_value > 1)
			spp_value = 1;
		spp = data->scene->noise_reduction * spp_value;
	}
	return (spp);
}

static void	render_ray(const t_ray *ray, const t_scene *scene, t_color_hdr *hdr)
{
	t_color_hdr			current;

	trace_color(scene, ray, 0, &current);
	hdr->r += current.r;
	hdr->g += current.g;
	hdr->b += current.b;
}

//t_vec2 next = (t_vec2) { ft_randf(), ft_randf() };
// so, subtract 0.5 from that to get a range from -0.5 to 0.5
// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space)
//gets a range of 0 to 1
// every ray gets 1/anti_aliasing of width
// I want this pixel to be divided into anti_aliasing^2 equally sized squares

static void	render_aa_pixel(t_mlx_data *data, int x, int y, int spp)
{
	int					i;
	t_ray				ray;
	t_vec2				next;
	t_temp_pixel_data	*pixel_data;
	t_color_hdr			*hdr;

	pixel_data = &data->renderer.temp_pixels[x + y * data->scene
		->resolution.width];
	hdr = &pixel_data->pixel_data.color;
	*hdr = (t_color_hdr){0, 0, 0 };
	i = 0;
	while (i < spp)
	{
		next = hilbert_randomizer_get_next(&pixel_data->randomizer);
		pix_to_ray(data,
			x + next.x - 0.5,
			y + next.y - 0.5,
			&ray
			);
		render_ray(&ray, data->scene, hdr);
		i++;
	}
	pixel_data->pixel_data.num_samples = spp;
}

void	render_pixel(t_mlx_data *data, int x, int y)
{
	if (is_first_frame(&data->renderer))
		render_first_pixel(data, x, y);
	else
		render_aa_pixel(data, x, y, calculate_spp(data, x, y));
}
