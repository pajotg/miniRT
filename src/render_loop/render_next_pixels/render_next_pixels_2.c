#include "stupid_norm.h"

// we already did the first one
// init values
// next frame will be a AA frame
// If we started rendering AA and then moved, we can have 0 samples in
//temp_pixels, dont copy that to the pixels
//data->renderer.pixels[i].num_samples = data
//->renderer.temp_pixels[i].pixel_data.num_samples;
//data->renderer.pixels[i].color =
//data->renderer.temp_pixels[i].pixel_data.color;

int	copy_temp_to_real(t_mlx_data *data)
{
	int	total_samples;
	int	i;

	i = 0;
	total_samples = 0;
	i = 0;
	while (i < data->scene->resolution.width * data->scene->resolution.height)
	{
		if (data->renderer.temp_pixels[i].pixel_data.num_samples > 0)
		{
			data->renderer.pixels[i] = data->renderer.temp_pixels[i].pixel_data;
			total_samples += data
				->renderer.temp_pixels[i].pixel_data.num_samples;
		}
		i++;
	}
	return (total_samples);
}

int	complete_first_frame(t_mlx_data *data)
{
	int	i;
	int	total_samples;

	data->renderer.avg_noise = 0;
	total_samples = copy_temp_to_real(data);
	if (data->renderer.frame_num == 1)
	{
		i = 0;
		while (i < data->scene->resolution.width * data->scene
			->resolution.height)
		{
			data->renderer.temp_pixels[i].aa_difference = 0;
			hilbert_randomizer_init_custom(&data
				->renderer.temp_pixels[i].randomizer, 0, 0, 1);
			i++;
		}
	}
	return (total_samples);
}

// Had some problems with nans when there is no ambiant, so... this should be 
//able to handle zero brightness

static float	safe_divide(float a, float b)
{
	if (b != 0)
		return (a / b);
	return (100000);
}

// divide by number of samples, to get a more accurate "how much noise does one
// sample give?"
// divide by magnitide of color, small differences in light spots dont matter,
//but in dark spots they matter alot!

int	complete_aa_pixel(t_mlx_data *data, int i)
{
	t_temp_pixel_data	*temp;
	t_pixel_data		*pixel;
	float				new_aa_difference;
	t_color_hdr			before;
	t_color_hdr			after;

	temp = &data->renderer.temp_pixels[i];
	pixel = &data->renderer.pixels[i];
	before = convert_to_hdr(pixel);
	pixel->color.r += temp->pixel_data.color.r;
	pixel->color.g += temp->pixel_data.color.g;
	pixel->color.b += temp->pixel_data.color.b;
	pixel->num_samples += temp->pixel_data.num_samples;
	after = convert_to_hdr(pixel);
	new_aa_difference = get_difference(&before, &after);
	new_aa_difference = safe_divide(new_aa_difference, (after.r * after.r
				 + after.g * after.g + after.b
				 * after.b) * temp->pixel_data.num_samples);
	if (new_aa_difference > 10)
		new_aa_difference = 10;
	if (new_aa_difference < 0)
		new_aa_difference = 0;
	temp->aa_difference = (new_aa_difference + temp->aa_difference) / 2;
	return (temp->pixel_data.num_samples);
}

int	complete_aa_frame(t_mlx_data *data, float *avg_noise)
{
	int					i;
	t_temp_pixel_data	*temp;
	int					total_samples;

	total_samples = 0;
	i = 0;
	while (i < data->scene->resolution.width * data->scene->resolution.height)
	{
		temp = &data->renderer.temp_pixels[i];
		if (temp->pixel_data.num_samples > 0)
			total_samples += complete_aa_pixel(data, i);
		*avg_noise += temp->aa_difference;
		i++;
	}
	*avg_noise /= data->scene->resolution.width * data->scene
		->resolution.height;
	return (total_samples);
}
