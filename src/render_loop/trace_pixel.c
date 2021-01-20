/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_pixel.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 13:59:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/20 15:52:36 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_raytracing.h"
#include "mini_rt_color_math_utils.h"
#include "ft_time.h"
#include "libft.h"
#include <stdio.h>
#include "math.h"

#include <stdlib.h>

static void get_hdr(t_mlx_data* data, int x, int y, t_color_hdr* out)
{
	t_pixel_data* pixel = &data->pixels[x + y * data->scene->resolution.width];
	if (pixel->num_samples == 0)
	{
		*out = (t_color_hdr) { 0, 0, 0 };
		return;
	}
	out->r = pixel->color.r / pixel->num_samples;
	out->g = pixel->color.g / pixel->num_samples;
	out->b = pixel->color.b / pixel->num_samples;
}

static float get_difference(t_color_hdr* col_a, t_color_hdr* col_b)
{
	float r = col_a->r - col_b->r;
	float g = col_a->g - col_b->g;
	float b = col_a->b - col_b->b;

	return (r * r + g * g + b * b);
}

static float get_edgyness(t_mlx_data* data, int x, int y)
{
	float edgyness = 0;

	t_color_hdr center;
	t_color_hdr other;
	get_hdr(data, x, y, &center);

	//printf("center hdr: %.2f %.2f %.2f\n", center.r, center.g, center.b);
	for (int ox = -1; ox <= 1; ox++)
	{
		if (x + ox < 0 || x + ox >= data->scene->resolution.width)
			continue;
		for (int oy = -1; oy <= 1; oy++)
		{
			if (y + oy < 0 || y + oy >= data->scene->resolution.height)
				continue;
			if (ox == 0 && oy == 0)
				continue;
			get_hdr(data, x + ox, y + oy, &other);
			edgyness += get_difference(&center, &other);
			//printf("	Got edgyness: %.2f for pixel: %i, %i\n", edgyness, x + ox, y + oy);
			//printf("	other hdr: %.2f %.2f %.2f\n", other.r, other.g, other.b);
		}
	}

	//printf("Got edgyness: %.2f for pixel: %i, %i\n", edgyness, x, y);
	//exit(1);

	// Apply a sigmoid function to map it to a 0-1 range
	//edgyness = edgyness / sqrtf(1 + edgyness * edgyness);
	return edgyness;
}

static void update_pix(t_mlx_data* data, int x, int y)
{
	t_color_hdr hdr;
	get_hdr(data, x, y, &hdr);

	t_color_rgb rgb = color_hdr_to_rgb_reindard_white(hdr, data->white);
	unsigned int col = (rgb.b | rgb.g << 8) | rgb.r << 16;

	size_t offset = x * (data->img.bits_per_pixel / 8) + y * data->img.line_length;
	unsigned int* addr = (unsigned int*)(data->img.addr + offset);
	*addr = col;
}

void update_image(t_mlx_data* data)
{
	for (int x = 0; x < data->img.width; x++)
		for (int y = 0; y < data->img.height; y++)
			update_pix(data, x, y);
}

/*
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
*/

void trace_pixel(t_mlx_data* data, int x, int y)
{
	// I want this pixel to be divided into anti_aliasing^2 equally sized squares
	// every ray gets 1/anti_aliasing of width
	// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space) gets a range of 0 to 1
	// so, subtract 0.5 from that to get a range from -0.5 to 0.5
	t_pixel_data* pixel_data = &data->pixels[x + y * data->scene->resolution.width];
	t_color_hdr* hdr = &pixel_data->color;
	t_ray ray;
	t_color_hdr current;

	int spp;
	if (pixel_data->num_samples == 0 || get_edgyness(data, x,y) < 0.01)	// 1 spp for low edgyness
		spp = 1;	// first frame = 1 spp, for faster initial frame, afterwards we can do more for better AA
	else
		spp = data->scene->samples_per_pixel;

	for (int i = 0; i < spp; i++)
	{
		pix_to_ray(data,
			x + ft_randf() - 0.5,
			y + ft_randf() - 0.5,
			&ray
		);
		trace_color(data, &ray, &current);
		hdr->r += current.r;
		hdr->g += current.g;
		hdr->b += current.b;
	}
	pixel_data->num_samples += spp;

	update_pix(data, x, y);
}

void trace_next_pixels(t_mlx_data* data, int desired)
{
	pthread_mutex_lock(&data->lock);
	// Properly init last_frame
	static t_time last_frame = { 0, 0 };
	if (last_frame.seconds == 0)
		last_frame = time_now();

	// Make sure every pixel is only traced once
	int pix = data->current_pixel;
	data->current_pixel+=desired;

	// Restarting frame rendering
	if (data->current_pixel >= data->scene->resolution.width * data->scene->resolution.height)
	{
		data->current_pixel = 0;

		// Print out how long it took
		t_time current = time_now();
		float diff = time_difference(&current, &last_frame);
		last_frame = current;
		printf("Completed frame! time taken: %.2fs \n", diff);
	}
	pthread_mutex_unlock(&data->lock);
	// Clamp stop to screen border
	int stop = pix+desired;
	if (stop > data->scene->resolution.width * data->scene->resolution.height)
		stop = data->scene->resolution.width * data->scene->resolution.height;

	// Trace
	for (int i = pix; i < stop; i++)
		trace_pixel(data, i % data->scene->resolution.width, i / data->scene->resolution.width);
}

void* new_pixel_render_thread(void* p)
{
	t_mlx_data* data;

	data = p;
	while (data->active)
		trace_next_pixels(data, 1000);
	return (NULL);
}
