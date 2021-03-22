/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/03/22 14:31:44 by jsimonis      ########   odam.nl         */
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
//#include "mlx_int.h"
#include <math.h>
#include "ft_printf.h"
#include "ft_error.h"
#include "ft_time.h"	// Used for movement (bonus)
#include "ft_manual_reset_event.h"
#include <stdio.h>

#define NUM_THREADS 5

void correct_exit(t_mlx_data* data)
{
	data->active = false;	// Notify render threads to stop
	// TODO: Figure out how to do this
	#ifdef OS_Linux
	mlx_loop_end(data->mlx);
	#else
	if (data->window)
	{
		printf("Oi, fix this!\n");
		exit(1);
	}
	#endif
}

static void cap_resolution(void* mlx, t_scene* scene)
{
	// TODO: Figure out how to do this
	#ifdef OS_Linux
	int rx, ry;
	mlx_get_screen_size(mlx, &rx, &ry);
	if (scene->resolution.width > rx)
		scene->resolution.width = rx;
	if (scene->resolution.height > ry)
		scene->resolution.height = ry;
	#else
	(void)mlx;
	(void)scene;
	#endif
}

static void do_loop(t_mlx_data* data)
{
	if (data->window)
	{
		mlx_loop(data->mlx);

		// Bit of sanity checking
		if (data->active)
		{
			ft_printf("Error\nWe called mlx_loop_end but not set active to false!\n");
			data->active = false;
		}
	}
	else
		while (data->active)
			render_next_pixels(data, 250);
}

static int do_error()
{
	ft_printf("Error\n%s\n",get_last_error());
	return (1);
}

int main(int argc, char **argv)
{
	//for (int i = 0; i < 1000; i++)
	//{
	//	float x = (ft_randf() - 0.5) * 10;
	//	float y = (ft_randf() - 0.5) * 10;
	//	float z = ft_randf() * 5;
	//
	//	int r = ft_rand() & 0xff;
	//	int g = ft_rand() & 0xff;
	//	int b = ft_rand() & 0xff;
	//
	//	t_vec3 dir = { ft_randf() - 0.5f, ft_randf() - 0.5f, ft_randf() - 0.5f };
	//	vec3_normalize(&dir, &dir);
	//
	//	printf("obj %.2f,%.2f,%.2f %.2f,%.2f,%.2f	../obj/Deer.obj D %i,%i,%i\n", x, y, z, dir.x, dir.y, dir.z, r,g,b);
	//}

	//return 1;
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
	{
		// TODO: Figure out hook keys
		//mlx_hook(mlx_data.window, KeyPress, KeyPressMask, &hook_key_down, &mlx_data);
		//mlx_hook(mlx_data.window, KeyRelease, KeyReleaseMask, &hook_key_up, &mlx_data);
		//mlx_mouse_hook(mlx_data.window, hook_mouse, &mlx_data);
		//mlx_hook(mlx_data.window, ClientMessage, StructureNotifyMask, hook_client_message, &mlx_data);
		mlx_loop_hook(mlx, hook_loop, &mlx_data);
	}

	pthread_t thread_ids[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&thread_ids[i], NULL, new_pixel_render_thread, &mlx_data);

	do_loop(&mlx_data);

	if (arg_data->save_on_exit)	// Save screenshot if requested
		if (!save_image(&mlx_data.img, "screenshot.bmp"))
			ft_printf("Error\nAn error occured while saving the screenshot: \"%s\"!\n", get_last_error());

	// may not be %100 thread safe:
	//		mlx loop has stopped, so that is thread safe
	//		waiting rendering threads see that active == false after being woken up, and return, so that is thread safe
	//		currently rendering threads can think that this is the last pixel, and reset rendering_done_mre, so that is NOT GOOD
	//			current "solution": have the reset code check mlx_data.active, and not reset if that is the case
	//			so:
	//				correct_exit > active = false > end mlx loop > set rendering_done_mre in lock > wait
	//				so if we are rendering the last pixels, we wait untill that is done, then set rendering_done_mre
	//				if we set rendering_done_mre, then render the last pixels, it should see that active == false and return nicely

	pthread_mutex_lock(&mlx_data.renderer.start_thread_lock);
	manual_reset_event_set(&mlx_data.renderer.rendering_done_mre);	// say that rendering is done, so they dont wait on that
	pthread_mutex_unlock(&mlx_data.renderer.start_thread_lock);
	for (int i = 0; i < NUM_THREADS; i++)	// Wait untill render threads are done
		pthread_join(thread_ids[i], NULL);

	// free stuff
	mlx_data_un_init(&mlx_data);
	// TODO: Figure out how to do this
	#ifdef OS_Linux
	mlx_destroy_display(mlx);
	#endif
	free(mlx);
	free_scene(scene);
	free(arg_data);

	ft_printf("Completed!\n");
	return 0;
}