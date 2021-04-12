/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jasper <jasper@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/22 18:24:12 by jasper        #+#    #+#                 */
/*   Updated: 2021/04/12 15:38:27 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "stupid_norm.h"

#if BONUS
# define NUM_THREADS 5
#else
# define NUM_THREADS 0
#endif

static pthread_t	g_thread_ids[NUM_THREADS];

// may not be %100 thread safe:
//		we are the mlx loop, so that is thread safe
//		waiting rendering threads see that active == false after being woken
//up, and return, so that is thread safe
//		currently rendering threads can think that this is the last pixel, and
//reset rendering_done_mre, so that is NOT GOOD
//			current "solution": have the reset code check mlx_data.active, and
//not reset if that is the case
//			so:
//				correct_exit > active = false > end mlx loop > set
//rendering_done_mre in lock > wait
//				so if we are rendering the last pixels, we wait untill that is
//done, then set rendering_done_mre
//				if we set rendering_done_mre, then render the last pixels, it
//should see that active == false and return nicely

// say that rendering is done, so they dont wait on that
// Wait untill render threads are done
// Save screenshot if requested
// free stuff
// We cant exit out of mlx_loop, so just exit(0)

void	mlx_loop_on_exit(t_mlx_data *data)
{
	int	i;

	pthread_mutex_lock(&data->renderer.start_thread_lock);
	manual_reset_event_set(&data->renderer.rendering_done_mre);
	pthread_mutex_unlock(&data->renderer.start_thread_lock);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(g_thread_ids[i], NULL);
		i++;
	}
	if (data->args->save_on_exit)
		if (!save_image(&data->img, "screenshot.bmp"))
			ft_printf("Error\nAn error occured while saving the screenshot: \"%"
				"s\"!\n", get_last_error());
	mlx_data_un_init(data);
	mlx_destroy_display_but_only_on_linux(data->mlx);
	free(data->mlx);
	free_scene(data->scene);
	free(data->args);
	ft_printf("Completed!\n");
	exit(0);
}

// Notify render threads & mlx loop to stop

void	correct_exit(t_mlx_data *data)
{
	data->active = false;
}

static int	do_error(void)
{
	ft_printf("Error\n%s\n", get_last_error());
	return (1);
}

// TODO: Figure out how to do this

static int	main_2(t_args *arg_data, t_scene *scene, void *mlx)
{
	int			i;
	t_mlx_data	mlx_data;

	if (!mlx_data_init(&mlx_data, mlx, scene, arg_data))
	{
		free(arg_data);
		free_scene(scene);
		mlx_destroy_display_but_only_on_linux(mlx);
		free(mlx);
		return (do_error());
	}
	if (mlx_data.window)
		hook_events(&mlx_data);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&g_thread_ids[i], NULL, new_pixel_render_thread,
			&mlx_data);
		i++;
	}
	do_loop(&mlx_data);
	correct_exit(&mlx_data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_args	*arg_data;
	t_scene	*scene;
	void	*mlx;

	init_ft_printf();
	arg_data = parse_args(argc, argv);
	if (!arg_data)
		return (do_error());
	scene = parse_scene(arg_data->map_file);
	if (!scene)
	{
		free(arg_data);
		return (do_error());
	}
	mlx = mlx_init();
	if (!mlx)
	{
		free(arg_data);
		free_scene(scene);
		return (do_error());
	}
	if (!arg_data->no_res_cap)
		cap_resolution(mlx, scene);
	return (main_2(arg_data, scene, mlx));
}
