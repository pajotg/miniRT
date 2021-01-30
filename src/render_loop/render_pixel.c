/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render_pixel.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/17 13:59:56 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/01/30 15:25:03 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "libft.h"
#include "mini_rt_render_pixel.h"
#include "mini_rt_hooks.h"
#include "mini_rt_image.h"
#include "ft_list.h"

void render_pixel(t_mlx_data* data, int x, int y)
{
	// I want this pixel to be divided into anti_aliasing^2 equally sized squares
	// every ray gets 1/anti_aliasing of width
	// so the final offset = 1/anti_aliasing * i + 0.5/anti_aliasing (pixel space) gets a range of 0 to 1
	// so, subtract 0.5 from that to get a range from -0.5 to 0.5
	t_temp_pixel_data* pixel_data = &data->renderer.temp_pixels[x + y * data->scene->resolution.width];
	t_color_hdr* hdr = &pixel_data->pixel_data.color;
	t_ray ray;

	if (is_first_frame(&data->renderer))
	{
		// for the first frame, dont use random scattering
		pix_to_ray(data, x, y, &ray);
		trace_color(data->scene, &ray, 0, hdr);
		pixel_data->pixel_data.num_samples = 1;
		// Fast render update for first frame
		write_pix(data, x, y, convert_to_hdr(&pixel_data->pixel_data));
	}
	else
	{
		int spp;
		int aa_index = get_aa_frame(&data->renderer, data->scene);
		if (aa_index != -1)	// AA frame
		{
			float edgyness = get_edgyness(data, x,y) * 15;
			if (edgyness > 1)
				edgyness = 1;
			spp = *(int*)list_index(&data->scene->samples_per_pixel, aa_index) * edgyness;
			if (spp <= 0 && aa_index == 0)
				spp = 1;
		} else {	// Noise reduction frame
			//float noise = pixel_data->aa_difference * 150;
			//float noise = get_noisyness(data, x,y) * 25;
			int nr_frame = data->renderer.frame_num - 2 - data->scene->samples_per_pixel.count;
			float noise = get_noisyness(data, x,y) * (7 + nr_frame * 0.7);
			if (noise > 1)
				noise = 1;
			spp = 64 * noise;	// Gotta make a seperated NR property
		}

		*hdr = (t_color_hdr) { 0,0,0 };
		for (int i = 0; i < spp; i++)
		{
			pix_to_ray(data,
				x + ft_randf() - 0.5,
				y + ft_randf() - 0.5,
				&ray
			);
			t_color_hdr current;
			trace_color(data->scene, &ray, 0, &current);
			hdr->r += current.r;
			hdr->g += current.g;
			hdr->b += current.b;
		}
		pixel_data->pixel_data.num_samples = spp;
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

		bool first_frame = is_first_frame(&data->renderer);
		int	total_samples = 0;
		float avg_noise = 0;
		if (first_frame)
		{
			for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
				if (data->renderer.temp_pixels[i].pixel_data.num_samples > 0)	// If we started rendering AA and then moved, we can have 0 samples in temp_pixels, dont copy that to the pixels
				{
					data->renderer.pixels[i].color = data->renderer.temp_pixels[i].pixel_data.color;
					data->renderer.pixels[i].num_samples = data->renderer.temp_pixels[i].pixel_data.num_samples;
					total_samples += data->renderer.temp_pixels[i].pixel_data.num_samples;
				}
			if (data->renderer.frame_num == 1)	// next frame will be a AA frame
				for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
					data->renderer.temp_pixels[i].aa_difference = 0;	// init values
		}
		else
			for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
			{
				t_temp_pixel_data *temp = &data->renderer.temp_pixels[i];

				if (temp->pixel_data.num_samples > 0)
				{
					t_pixel_data *pixel = &data->renderer.pixels[i];
					t_color_hdr before = convert_to_hdr(pixel);

					pixel->color.r += temp->pixel_data.color.r;
					pixel->color.g += temp->pixel_data.color.g;
					pixel->color.b += temp->pixel_data.color.b;
					pixel->num_samples += temp->pixel_data.num_samples;

					t_color_hdr after = convert_to_hdr(pixel);
					temp->aa_difference = get_difference(&before, &after);
					total_samples += temp->pixel_data.num_samples;
				}
				avg_noise += temp->aa_difference;
			}
		avg_noise /= data->scene->resolution.width * data->scene->resolution.height;
		pthread_mutex_lock(&data->renderer.hook_thread_lock);
		hook_frame_complete(data, total_samples, avg_noise);
		pthread_mutex_unlock(&data->renderer.hook_thread_lock);

		data->renderer.frame_num++;
		if (data->scene->samples_per_pixel.count <= 0)	// We dont go into a AA frame if there is no AA to be done
			data->renderer.frame_num = 1;
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
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
