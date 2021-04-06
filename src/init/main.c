/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/06 12:16:57 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt_mlx.h"
#include "mini_rt_hooks.h"
#include "mini_rt_render_pixel.h"
#include "mini_rt_init.h"
#include "mini_rt_object.h"

#include <stdlib.h>
#include "ft_printf.h"
#include "mini_rt_bmp.h"
#include "libft.h"
#include "mlx.h"
#ifdef OS_Linux
#include "mlx_int.h"
#endif
#include <math.h>
#include "ft_printf.h"
#include "ft_error.h"
#include "ft_time.h"	// Used for movement (bonus)
#include "ft_manual_reset_event.h"
#include <stdio.h>

#define NUM_THREADS 5

static pthread_t g_thread_ids[NUM_THREADS];

void mlx_loop_on_exit(t_mlx_data* data)
{
	// may not be %100 thread safe:
	//		we are the mlx loop, so that is thread safe
	//		waiting rendering threads see that active == false after being woken up, and return, so that is thread safe
	//		currently rendering threads can think that this is the last pixel, and reset rendering_done_mre, so that is NOT GOOD
	//			current "solution": have the reset code check mlx_data.active, and not reset if that is the case
	//			so:
	//				correct_exit > active = false > end mlx loop > set rendering_done_mre in lock > wait
	//				so if we are rendering the last pixels, we wait untill that is done, then set rendering_done_mre
	//				if we set rendering_done_mre, then render the last pixels, it should see that active == false and return nicely

	pthread_mutex_lock(&data->renderer.start_thread_lock);
	manual_reset_event_set(&data->renderer.rendering_done_mre);	// say that rendering is done, so they dont wait on that
	pthread_mutex_unlock(&data->renderer.start_thread_lock);

	for (int i = 0; i < NUM_THREADS; i++)	// Wait untill render threads are done
		pthread_join(g_thread_ids[i], NULL);

	if (data->args->save_on_exit)	// Save screenshot if requested
		if (!save_image(&data->img, "screenshot.bmp"))
			ft_printf("Error\nAn error occured while saving the screenshot: \"%s\"!\n", get_last_error());

	// free stuff
	mlx_data_un_init(data);
	// TODO: Figure out how to do this
	#ifdef OS_Linux
	mlx_destroy_display(data->mlx);
	#endif
	free(data->mlx);
	free_scene(data->scene);
	free(data->args);

	ft_printf("Completed!\n");
	exit(0);	// We cant exit out of mlx_loop, so just exit(0)
}

void correct_exit(t_mlx_data* data)
{
	data->active = false;	// Notify render threads to stop
}

static void cap_resolution(void* mlx, t_scene* scene)
{
	int rx, ry;
	mlx_get_screen_size(mlx, &rx, &ry);
	if (scene->resolution.width > rx)
		scene->resolution.width = rx;
	if (scene->resolution.height > ry)
		scene->resolution.height = ry;
}

static void do_loop(t_mlx_data* data)
{
	if (data->window)
		mlx_loop(data->mlx);
	else
		while (data->active)
			render_next_pixels(data, 250);
	mlx_loop_on_exit(data);
}

static int do_error()
{
	ft_printf("Error\n%s\n",get_last_error());
	return (1);
}

static void hook_events(t_mlx_data* mlx_data)
{
	#ifdef OS_Linux
	mlx_hook(mlx_data->window, KeyPress, KeyPressMask, &hook_key_down, mlx_data);
	mlx_hook(mlx_data->window, KeyRelease, KeyReleaseMask, &hook_key_up, mlx_data);
	mlx_hook(mlx_data->window, ClientMessage, StructureNotifyMask, hook_client_message, mlx_data);
	#else
	mlx_hook(mlx_data->window, 2, 0, &hook_key_down, mlx_data);
	mlx_hook(mlx_data->window, 3, 0, &hook_key_up, mlx_data);
	mlx_hook(mlx_data->window, 17, 0, hook_client_message, mlx_data);
	#endif
	mlx_mouse_hook(mlx_data->window, hook_mouse, mlx_data);
	mlx_loop_hook(mlx_data->mlx, hook_loop, mlx_data);
}

int main(int argc, char **argv)
{
	init_ft_printf();

	t_args* arg_data = parse_args(argc, argv);
	if (!arg_data)
		return do_error();

	t_scene* scene = parse_scene(arg_data->map_file);
	if (!scene)
	{
		free(arg_data);
		return do_error();
	}

	void* mlx = mlx_init();
	if (!mlx)
	{
		free(arg_data);
		free_scene(scene);
		return do_error();
	}

	if (!arg_data->no_res_cap)
		cap_resolution(mlx, scene);

	t_mlx_data mlx_data;
	if (!mlx_data_init(&mlx_data, mlx, scene, arg_data))
	{
		free(arg_data);
		free_scene(scene);
		// TODO: Figure out how to do this
		#ifdef OS_Linux
		mlx_destroy_display(mlx);
		#endif
		free(mlx);
		return do_error();
	}

	if (mlx_data.window)
		hook_events(&mlx_data);

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&g_thread_ids[i], NULL, new_pixel_render_thread, &mlx_data);

	do_loop(&mlx_data);

	correct_exit(&mlx_data);

	return 0;
}