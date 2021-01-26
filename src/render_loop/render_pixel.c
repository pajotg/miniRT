/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render_pixel.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 13:59:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/26 18:23:23 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "libft.h"
#include "mini_rt_render_pixel.h"
#include "mini_rt_hooks.h"
#include "mini_rt_image.h"

void render_pixel(t_mlx_data* data, int x, int y)
{
	// I want this pixel to be divided into anti_aliasing^2 equally sized squares
	// every ray gets 1/anti_aliasing of width
	// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space) gets a range of 0 to 1
	// so, subtract 0.5 from that to get a range from -0.5 to 0.5
	t_pixel_data* pixel_data = &data->renderer.temp_pixels[x + y * data->scene->resolution.width];
	t_color_hdr* hdr = &pixel_data->color;
	t_ray ray;

	if (data->renderer.first_frame)
	{
		// for the first frame, dont use random scattering
		pix_to_ray(data, x, y, &ray);
		trace_color(data->scene, &ray, hdr);
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
			trace_color(data->scene, &ray, &current);
			hdr->r += current.r;
			hdr->g += current.g;
			hdr->b += current.b;
		}
		pixel_data->num_samples = spp;
	}
}

void render_next_pixels(t_mlx_data* data, int desired)
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
		render_pixel(data, i % data->scene->resolution.width, i / data->scene->resolution.width);

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
			else if (data->scene->samples_per_pixel > 0)	// We dont go into a AA frame if there is no AA to be done
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
		render_next_pixels(data, 2500);
	return (NULL);
}
