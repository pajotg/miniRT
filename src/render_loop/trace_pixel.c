/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trace_pixel.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 13:59:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/21 15:02:50 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_raytracing.h"
#include "mini_rt_color_math_utils.h"
#include "ft_time.h"
#include "libft.h"
#include <stdio.h>
#include "math.h"
#include "mini_rt_hooks.h"

#include <stdlib.h>

static t_color_hdr convert_to_hdr(t_pixel_data* pixel)
{
	t_color_hdr out;
	if (pixel->num_samples == 0)
	{
		out = (t_color_hdr) { 0,0,0 };
		return (out);
	}
	out.r = pixel->color.r / pixel->num_samples;
	out.g = pixel->color.g / pixel->num_samples;
	out.b = pixel->color.b / pixel->num_samples;
	return (out);
}

static t_color_hdr get_hdr(t_mlx_data *data, int x, int y)
{
	return (convert_to_hdr(&data->renderer.pixels[x + y * data->scene->resolution.width]));
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

	t_color_hdr center = get_hdr(data, x, y);
	t_color_hdr other;

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
			other = get_hdr(data, x + ox, y + oy);
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

static void write_pix(t_mlx_data* data, int x, int y, t_color_hdr hdr)
{
	t_color_rgb rgb = color_hdr_to_rgb_reindard_white(hdr, data->white);
	unsigned int col = (rgb.b | rgb.g << 8) | rgb.r << 16;

	size_t offset = x * (data->img.bits_per_pixel / 8) + y * data->img.line_length;
	unsigned int* addr = (unsigned int*)(data->img.addr + offset);
	*addr = col;
}

static void update_pix(t_mlx_data* data, int x, int y)
{
	t_color_hdr hdr = get_hdr(data, x, y);
	write_pix(data, x, y, hdr);
}

void update_image(t_mlx_data* data)
{
	for (int x = 0; x < data->img.width; x++)
		for (int y = 0; y < data->img.height; y++)
			update_pix(data, x, y);
}

void trace_pixel(t_mlx_data* data, int x, int y)
{
	// I want this pixel to be divided into anti_aliasing^2 equally sized squares
	// every ray gets 1/anti_aliasing of width
	// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space) gets a range of 0 to 1
	// so, subtract 0.5 from that to get a range from -0.5 to 0.5
	t_pixel_data* pixel_data = &data->renderer.temp_pixels[x + y * data->scene->resolution.width];
	t_color_hdr* hdr = &pixel_data->color;
	t_ray ray;

	if (data->renderer.first_frame)	// first frame = 1spp
	{
		pix_to_ray(data, x, y, &ray);
		trace_color(data, &ray, hdr);
		pixel_data->num_samples = 1;
		// Fast render update for first frame
		write_pix(data, x, y, convert_to_hdr(pixel_data));
	}
	else
	{
		float edgyness = get_edgyness(data, x,y) * 5;
		if (edgyness > 1)
			edgyness = 1;
		int spp = data->scene->samples_per_pixel * edgyness;

		*hdr = (t_color_hdr) { 0,0,0 };
		for (int i = 0; i < spp; i++)
		{
			pix_to_ray(data,
				x + ft_randf() - 0.5,
				y + ft_randf() - 0.5,
				&ray
			);
			t_color_hdr current;
			trace_color(data, &ray, &current);
			hdr->r += current.r;
			hdr->g += current.g;
			hdr->b += current.b;
		}
		pixel_data->num_samples = spp;
	}
}

void trace_next_pixels(t_mlx_data* data, int desired)
{
	pthread_mutex_lock(&data->renderer.start_thread_lock);

	// Make sure every pixel is only traced once
	int pix = data->renderer.current_pixel;
	data->renderer.current_pixel+=desired;

	// Check if we are the last or first render thread for this frame
	bool is_first_render_thread = pix == 0;
	bool is_last_render_thread = data->renderer.current_pixel >= data->scene->resolution.width * data->scene->resolution.height;

	// Only say that this thread is rendering once we have completed the frame
	pthread_mutex_lock(&data->renderer.active_render_threads_lock);
	if (data->renderer.active_render_threads == 0)
		manual_reset_event_reset(&data->renderer.no_render_threads_mre);
	data->renderer.active_render_threads++;
	pthread_mutex_unlock(&data->renderer.active_render_threads_lock);

	pthread_mutex_lock(&data->renderer.hook_thread_lock);
	// By keeping it locked we prevent new threads from starting, we should be the last for this frame
	if (!is_last_render_thread)
		pthread_mutex_unlock(&data->renderer.start_thread_lock);

	if (is_first_render_thread)
		hook_frame_start(data);
	pthread_mutex_unlock(&data->renderer.hook_thread_lock);

	// Clamp stop to screen border
	int stop = pix+desired;
	if (stop > data->scene->resolution.width * data->scene->resolution.height)
		stop = data->scene->resolution.width * data->scene->resolution.height;

	// Trace
	for (int i = pix; i < stop; i++)
		trace_pixel(data, i % data->scene->resolution.width, i / data->scene->resolution.width);

	// Say that we are done rendering this frame
	pthread_mutex_lock(&data->renderer.active_render_threads_lock);
	data->renderer.active_render_threads--;
	if (data->renderer.active_render_threads == 0)
		manual_reset_event_set(&data->renderer.no_render_threads_mre);
	pthread_mutex_unlock(&data->renderer.active_render_threads_lock);

	if (is_last_render_thread)
	{
		// Restarting frame rendering
		manual_reset_event_wait(&data->renderer.no_render_threads_mre);	// Wait untill all render threads have stopped

		// Alright! no threads are rendering, time to copy!
		data->renderer.current_pixel = 0;

		bool first_frame = data->renderer.first_frame;
		if (first_frame)
		{
			if (data->renderer.dirty_frame)
				data->renderer.dirty_frame = false;
			else
				data->renderer.first_frame = false;
			for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
				if (data->renderer.temp_pixels[i].num_samples > 0)	// If we started rendering AA and then moved, we can have 0 samples in temp_pixels, dont copy that to the pixels
					data->renderer.pixels[i] = data->renderer.temp_pixels[i];
		}
		else
			for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
			{
				t_pixel_data *pixel = &data->renderer.pixels[i];
				t_pixel_data *temp = &data->renderer.temp_pixels[i];

				pixel->color.r += temp->color.r;
				pixel->color.g += temp->color.g;
				pixel->color.b += temp->color.b;
				pixel->num_samples += temp->num_samples;
			}
		pthread_mutex_lock(&data->renderer.hook_thread_lock);
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
		hook_frame_complete(data, first_frame);
		pthread_mutex_unlock(&data->renderer.hook_thread_lock);
	}
}

void* new_pixel_render_thread(void* p)
{
	t_mlx_data* data;

	data = p;
	while (data->active)
		trace_next_pixels(data, 2500);
	return (NULL);
}
