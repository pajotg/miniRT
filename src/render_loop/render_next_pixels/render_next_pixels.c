#include "stupid_norm.h"

// we must not want to exit
// must not be a dirty frame

static bool	is_last_frame(t_mlx_data *data)
{
	return (data->renderer.frame_num > 0
		 && ((size_t)(data->renderer.frame_num - 1) >= data->scene
		->samples_per_pixel.count) // next frame is not a AA frame
		&& data->scene->noise_reduction == 0 // there is no noise reduction
		&& data->active);
}

//	manual_reset_event_reset(&data->renderer.rendering_done_mre);
//if (data->renderer.frame_num > 1 && get_aa_frame(&data->renderer,
//data->scene) == -1 && data->scene->noise_reduction == 0 && data->active)
// Alright! no threads are rendering, time to copy!
// Wait untill all render threads have stopped
// Restarting frame rendering

static void	last_render_thread_of_frame(t_mlx_data *data)
{
	int		total_samples;
	bool	first_frame;
	float	avg_noise;

	manual_reset_event_wait(&data->renderer.no_render_threads_mre);
	data->renderer.current_pixel = 0;
	first_frame = is_first_frame(&data->renderer);
	avg_noise = 0;
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
	pthread_mutex_unlock(&data->renderer.start_thread_lock);
}

// Say that we are done rendering this frame
// Trace
// Clamp stop to screen border
// By keeping it locked we prevent new threads from starting, we should be the
//last for this frame
// Only say that this thread is rendering once we have completed the frame
// Check if we are the last or first render thread for this frame
// Make sure every pixel is only traced once

static void	calculate_rpd_datas(t_mlx_data *data, int desired,
	t_render_pixels_data *rpd)
{
	rpd->pix = data->renderer.current_pixel;
	data->renderer.current_pixel += desired;
	rpd->is_first_render_thread = rpd->pix == 0;
	rpd->is_last_render_thread = data->renderer.current_pixel >= data->scene
		->resolution.width * data->scene->resolution.height;
	pthread_mutex_lock(&data->renderer.active_render_threads_lock);
	if (data->renderer.active_render_threads == 0)
		manual_reset_event_reset(&data->renderer.no_render_threads_mre);
	data->renderer.active_render_threads++;
	pthread_mutex_unlock(&data->renderer.active_render_threads_lock);
	pthread_mutex_lock(&data->renderer.hook_thread_lock);
	if (!rpd->is_last_render_thread)
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
	if (rpd->is_first_render_thread)
		hook_frame_start(data);
	pthread_mutex_unlock(&data->renderer.hook_thread_lock);
	rpd->stop = rpd->pix + desired;
	if (rpd->stop > data->scene->resolution.width * data->scene
		->resolution.height)
		rpd->stop = data->scene->resolution.width * data->scene
			->resolution.height;
}

void	render_next_pixels(t_mlx_data *data, int desired)
{
	t_render_pixels_data	rpd;

	pthread_mutex_lock(&data->renderer.start_thread_lock);
	if (!data->renderer.rendering_done_mre.is_set)
	{
		pthread_mutex_unlock(&data->renderer.start_thread_lock);
		return ;
	}
	calculate_rpd_datas(data, desired, &rpd);
	rpd.i = rpd.pix;
	while (rpd.i < rpd.stop)
	{
		render_pixel(data, rpd.i % data->scene->resolution.width, rpd.i / data
			->scene
			->resolution.width);
		rpd.i++;
	}
	pthread_mutex_lock(&data->renderer.active_render_threads_lock);
	data->renderer.active_render_threads--;
	if (data->renderer.active_render_threads == 0)
		manual_reset_event_set(&data->renderer.no_render_threads_mre);
	pthread_mutex_unlock(&data->renderer.active_render_threads_lock);
	if (rpd.is_last_render_thread)
		last_render_thread_of_frame(data);
}
