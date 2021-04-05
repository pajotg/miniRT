#include "mini_rt_mlx.h"
#include "libft.h"
#include "mini_rt_render_pixel.h"
#include "mini_rt_hooks.h"
#include "mini_rt_image.h"
#include "ft_list.h"
#include <math.h>

#include <stdio.h>

static int complete_first_frame(t_mlx_data* data)
{
	int total_samples = 0;
	int i;

	data->renderer.avg_noise = 0;
	for (i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
	{
		if (data->renderer.temp_pixels[i].pixel_data.num_samples > 0)	// If we started rendering AA and then moved, we can have 0 samples in temp_pixels, dont copy that to the pixels
		{
			data->renderer.pixels[i].color = data->renderer.temp_pixels[i].pixel_data.color;
			data->renderer.pixels[i].num_samples = data->renderer.temp_pixels[i].pixel_data.num_samples;
			total_samples += data->renderer.temp_pixels[i].pixel_data.num_samples;
		}
	}
	if (data->renderer.frame_num == 1)	// next frame will be a AA frame
		for (i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
		{
			// init values
			data->renderer.temp_pixels[i].aa_difference = 0;
			hilbert_randomizer_init_custom(&data->renderer.temp_pixels[i].randomizer, 0, 0, 1);	// we already did the first one
		}
	return total_samples;
}

static int complete_aa_pixel(t_mlx_data* data, int i)
{
	t_temp_pixel_data *temp = &data->renderer.temp_pixels[i];
	t_pixel_data *pixel = &data->renderer.pixels[i];
	t_color_hdr before = convert_to_hdr(pixel);

	pixel->color.r += temp->pixel_data.color.r;
	pixel->color.g += temp->pixel_data.color.g;
	pixel->color.b += temp->pixel_data.color.b;
	pixel->num_samples += temp->pixel_data.num_samples;

	t_color_hdr after = convert_to_hdr(pixel);
	float new_aa_difference = get_difference(&before, &after);
	// divide by magnitide of color, small differences in light spots dont matter, but in dark spots they matter alot!
	new_aa_difference /= (after.r * after.r + after.g * after.g + after.b * after.b);
	// divide by number of samples, to get a more accurate "how much noise does one sample give?"
	new_aa_difference /= temp->pixel_data.num_samples;
	if (new_aa_difference > 10)
		new_aa_difference = 10;

	temp->aa_difference = (new_aa_difference + temp->aa_difference) / 2;

	return temp->pixel_data.num_samples;
}

static int complete_aa_frame(t_mlx_data* data, float* avg_noise)
{
	int total_samples = 0;
	for (int i = 0; i < data->scene->resolution.width * data->scene->resolution.height; i++)
	{
		t_temp_pixel_data *temp = &data->renderer.temp_pixels[i];
		if (temp->pixel_data.num_samples > 0)
			total_samples += complete_aa_pixel(data, i);
		*avg_noise += temp->aa_difference;
	}
	*avg_noise /= data->scene->resolution.width * data->scene->resolution.height;
	return total_samples;
}

static bool is_last_frame(t_mlx_data* data)
{
	return (data->renderer.frame_num > 0 &&	// must not be a dirty frame
		((size_t)(data->renderer.frame_num - 1) >= data->scene->samples_per_pixel.count)	// next frame is not a AA frame
		&& data->scene->noise_reduction == 0	// there is no noise reduction
		&& data->active);	// we must not want to exit
}

static void last_render_thread_of_frame(t_mlx_data* data)
{
	// Restarting frame rendering
	manual_reset_event_wait(&data->renderer.no_render_threads_mre);	// Wait untill all render threads have stopped

	// Alright! no threads are rendering, time to copy!
	data->renderer.current_pixel = 0;

	bool first_frame = is_first_frame(&data->renderer);
	float avg_noise = 0;
	int	total_samples;
	if (first_frame)
		total_samples = complete_first_frame(data);
	else
		total_samples = complete_aa_frame(data, &avg_noise);
	data->renderer.avg_noise = avg_noise;

	if (is_last_frame(data))
		manual_reset_event_reset(&data->renderer.rendering_done_mre);

	pthread_mutex_lock(&data->renderer.hook_thread_lock);
	hook_frame_complete(data, total_samples, avg_noise);
	pthread_mutex_unlock(&data->renderer.hook_thread_lock);

	data->renderer.frame_num++;
	//if (data->renderer.frame_num > 1 && get_aa_frame(&data->renderer, data->scene) == -1 && data->scene->noise_reduction == 0 && data->active)
	//	manual_reset_event_reset(&data->renderer.rendering_done_mre);
	pthread_mutex_unlock(&data->renderer.start_thread_lock);
}

void render_next_pixels(t_mlx_data* data, int desired)
{
	pthread_mutex_lock(&data->renderer.start_thread_lock);
	if (!data->renderer.rendering_done_mre.is_set)
	{
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
		return;
	}
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
		last_render_thread_of_frame(data);
}