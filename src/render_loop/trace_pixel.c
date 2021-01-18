/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_pixel.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 13:59:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/18 13:27:40 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_raytracing.h"
#include "mini_rt_color_math_utils.h"
#include "ft_time.h"
#include <stdio.h>

static void update_pix(t_mlx_data* data, int x, int y)
{
	t_color_hdr hdr = data->pixels[x + y * data->scene->resolution.width].color;
	t_color_rgb rgb = color_hdr_to_rgb_reindard_white(hdr, data->white);
	unsigned int col = (rgb.b | rgb.g << 8) | rgb.r << 16;

	size_t offset = x * (data->img.bits_per_pixel / 8) + y * data->img.line_length;
	unsigned int* addr = (unsigned int*)(data->img.addr + offset);
	*addr = col;
}

void update_image(t_mlx_data* data)
{
	// TODO: I should only update the pixels that have changed, this already takes 100 ms
	for (int x = 0; x < data->img.width; x++)
	{
		for (int y = 0; y < data->img.height; y++)
		{
			update_pix(data, x, y);
		}
	}
}

void trace_pixel(t_mlx_data* data, int x, int y)
{
	// I want this pixel to be divided into anti_aliasing^2 equally sized squares
	// every ray gets 1/anti_aliasing of width
	// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space) gets a range of 0 to 1
	// so, subtract 0.5 from that to get a range from -0.5 to 0.5
	t_color_hdr* hdr = &data->pixels[x + y * data->scene->resolution.width].color;
	t_ray ray;
	t_color_hdr current;
	hdr->r = 0;
	hdr->g = 0;
	hdr->b = 0;

	int aa = data->scene->anti_aliasing;
	float step = 1.0/aa;
	float offset = step / 2 - 0.5;
	for (int ox = 0; ox < aa; ox++)
	{
		for (int oy = 0; oy < aa; oy++)
		{
			pix_to_ray(data,
				x + step * ox + offset,
				y + step * ox + offset,
				&ray
			);
			trace_color(data, &ray, &current);
			hdr->r += current.r;
			hdr->g += current.g;
			hdr->b += current.b;
		}
	}

	float total_samples = aa * aa;
	hdr->r /= total_samples;
	hdr->g /= total_samples;
	hdr->b /= total_samples;
	update_pix(data, x, y);
}

void trace_next_pixels(t_mlx_data* data, int desired)
{
	pthread_mutex_lock(&data->lock);
	int pix = data->current_pixel;
	data->current_pixel+=desired;
	if (data->current_pixel >= data->scene->resolution.width * data->scene->resolution.height)
	{
		static t_time last = { 0, 0 };
		t_time current = time_now();
		float diff = time_difference(&current, &last);
		if (last.seconds == 0)
			diff = -1;
		last = current;
		data->current_pixel = 0;

		printf("Completed frame! time taken: %.2fs \n", diff);
	}
	pthread_mutex_unlock(&data->lock);
	int stop = pix+desired;
	if (stop > data->scene->resolution.width * data->scene->resolution.height)
		stop = data->scene->resolution.width * data->scene->resolution.height;
	for (int i = pix; i < stop; i++)
	{
		trace_pixel(data, i % data->scene->resolution.width, i / data->scene->resolution.width);
	}
}

void* new_pixel_render_thread(void* p)
{
	t_mlx_data* data = p;

	while (data->active)
		trace_next_pixels(data, 1000);
	return NULL;
}