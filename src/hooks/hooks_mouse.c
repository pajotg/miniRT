/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks_mouse.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/11 13:59:38 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/04/11 21:59:50 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//#include "mini_rt_mlx.h"
#include "mini_rt_object.h"
#include <stdio.h>
#include <math.h>
#include "ft_printf.h"
#include "mini_rt_render_pixel.h"

static void	look_at(t_mlx_data *data, int x, int y)
{
	float			sqrmag;
	t_quaternion	new_rot;
	t_vec3			up;
	t_camera		*cam;
	t_ray			ray;

	pix_to_ray(data, x, y, &ray);
	cam = list_index(&data->scene->cameras, data->scene->current_camera_index);
	sqrmag = vec3_magnitude_sqr(&ray.direction);
	if (sqrmag < 0.99 || sqrmag > 1.01)
	{
		printf("Bad ray create! %.2f = sqrmag != 1: %.2f %.2f %.2f\n", sqrmag,
			ray.direction.x, ray.direction.y, ray.direction.z);
	}
	if (data->input.camera_free)
	{
		quaternion_mult_vec3(&up, &cam->transform.rotation, vec3_up());
		quaternion_from_forward_up(&new_rot, &ray.direction, &up);
	}
	else
		quaternion_from_forward_up(&new_rot, &ray.direction, vec3_up());
	cam->transform.rotation = new_rot;
	data->should_clear = true;
}

static void	debug_ray_hit(t_ray ray, t_ray_hit hit)
{
	float		sqrmag;
	t_vec3		expected_loc;
	t_vec3		diff;

	printf("	Hit!\n");
	printf("		Location: %.2f %.2f %.2f!\n", hit.location.x,
		hit.location.y, hit.location.z);
	printf("		Distance: %.2f!\n", hit.distance);
	printf("		Normal: %.2f %.2f %.2f!\n", hit.normal.x, hit.normal.y,
		hit.normal.z);
	printf("		Object: %p!\n", hit.object);
	sqrmag = vec3_magnitude_sqr(&hit.normal);
	if (sqrmag > 1.01 || sqrmag < 0.99)
		printf("		Err: Normal magnitude != 1, actual: %.2f\n", sqrtf(
				sqrmag));
	vec3_scale(&expected_loc, &ray.direction, hit.distance);
	vec3_add(&expected_loc, &expected_loc, &ray.origin);
	vec3_subtract(&diff, &expected_loc, &hit.location);
	sqrmag = vec3_magnitude_sqr(&diff);
	if (sqrmag > 0.01)
		ft_printf("		Err: Expected location %v but got: %v!\n",
			&expected_loc, &hit.location);
	if (vec3_dot(&hit.normal, &ray.direction) > 0)
		ft_printf("		Err: hit.normal in opposite direction!\n");
}

static void	debug_ray(t_mlx_data *data, int x, int y)
{
	t_ray		ray;
	t_ray_hit	hit;

	pix_to_ray(data, x, y, &ray);
	printf("Ray dir: %.2f %.2f %.2f!\n", ray.direction.x, ray.direction.y,
		ray.direction.z);
	if (cast_ray(data->scene, &ray, &hit))
		debug_ray_hit(ray, hit);
	else
		printf("	Miss!\n");
}

static void	debug_pixel(t_mlx_data *data, int x, int y)
{
	t_pixel_data		*pixel_data;
	t_temp_pixel_data	*temp_pixel_data;
	t_color_hdr			hdr;
	t_color_hdr			temp_hdr;

	pixel_data = &data->renderer.pixels[x + y * data->scene->resolution.width];
	temp_pixel_data = &data->renderer.temp_pixels[x + y * data->scene
		->resolution.width];
	hdr = pixel_data->color;
	temp_hdr = temp_pixel_data->pixel_data.color;
	printf("pixel: Color: %.2f %.2f %.2f, mag: %.2f, num samples: %i\n", hdr.r,
		hdr.g, hdr.b,
		sqrtf(hdr.r * hdr.r + hdr.g * hdr.g + hdr.b * hdr.b),
		pixel_data->num_samples
		);
	printf("temp pixel: Color: %.2f %.2f %.2f, mag: %.2f, num samples: %i, nois"
		"e: %.8f\n", temp_hdr.r, temp_hdr.g, temp_hdr.b,
		sqrtf(temp_hdr.r * temp_hdr.r + temp_hdr.g * temp_hdr.g + temp_hdr.b
			 * temp_hdr.b),
		temp_pixel_data->pixel_data.num_samples, temp_pixel_data->aa_difference
		);
}

/*
**	1 (left click) = look at point
**	2 (middle click) = gather debug data from pixel
**	3 (right click) = get HDR color
*/

//printf("mouse hooked! %i at: (%i, %i) %p\n",button, x, y, p);

int	hook_mouse(int button, int x, int y, void *p)
{
	t_mlx_data	*data;

	data = p;
	if (button == 1)
		look_at(data, x, y);
	if (button == 2)
		debug_ray(data, x, y);
	else if (button == 3)
		debug_pixel(data, x, y);
	return (0);
}
