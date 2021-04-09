#include "stupid_norm.h"

void	cap_resolution(void *mlx, t_scene *scene)
{
	int	rx;
	int	ry;

	mlx_get_screen_size(mlx, &rx, &ry);
	if (scene->resolution.width > rx)
		scene->resolution.width = rx;
	if (scene->resolution.height > ry)
		scene->resolution.height = ry;
}

void	do_loop(t_mlx_data *data)
{
	if (data->window)
		mlx_loop(data->mlx);
	else
		while (data->active)
			render_next_pixels(data, 250);
	mlx_loop_on_exit(data);
}

// Uhggers, its cleaner to use the pre-processor statement inside the function
// That way you can easily see that mlx_mouse_hook & mlx_loop_hook are shared
// Then i would also not have duplicate code! (The norm agrees: thats bad!)
// Also easily see the similarities on what function binds to what
// Overall: the norm is OBJECTIVELY worse here

#ifdef OS_Linux
void	hook_events(t_mlx_data *mlx_data)
{
	mlx_hook(mlx_data->window, KeyPress, KeyPressMask, &hook_key_down, mlx_data)
	;
	mlx_hook(mlx_data->window, KeyRelease, KeyReleaseMask, &hook_key_up,
		mlx_data);
	mlx_hook(mlx_data->window, ClientMessage, StructureNotifyMask,
		hook_client_message, mlx_data);
	mlx_mouse_hook(mlx_data->window, hook_mouse, mlx_data);
	mlx_loop_hook(mlx_data->mlx, hook_loop, mlx_data);
}
#else

void	hook_events(t_mlx_data *mlx_data)
{
	mlx_hook(mlx_data->window, 2, 0, &hook_key_down, mlx_data);
	mlx_hook(mlx_data->window, 3, 0, &hook_key_up, mlx_data);
	mlx_hook(mlx_data->window, 17, 0, hook_client_message, mlx_data);
	mlx_mouse_hook(mlx_data->window, hook_mouse, mlx_data);
	mlx_loop_hook(mlx_data->mlx, hook_loop, mlx_data);
}
#endif