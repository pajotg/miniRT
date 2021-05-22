#include "stupid_norm.h"
#include "ft_ternary.h"

void	ric_extract_data(t_cube_intersection_data *sa, const t_object *object,
	const t_ray *ray)
{
	sa->data = object->object_data;
	quaternion_conjugate(&sa->conj, &object->transform.rotation);
	vec3_subtract(&sa->origin_vec, &ray->origin, &object->transform.position);
	quaternion_mult_vec3(&sa->origin_vec, &sa->conj, &sa->origin_vec);
	quaternion_mult_vec3(&sa->dir_vec, &sa->conj, &ray->direction);
	sa->origin = (float *)&sa->origin_vec;
	sa->dir = (float *)&sa->dir_vec;
	sa->i = 0;
	while (sa->i < 3)
	{
		sa->min_b[sa->i] = -sa->data->extends;
		sa->max_b[sa->i] = +sa->data->extends;
		sa->i++;
	}
}

void	ric_calculate_sides(t_cube_intersection_data *sa)
{
	sa->i = 0;
	sa->inside = true;
	while (sa->i < 3)
	{
		if (sa->origin[sa->i] < sa->min_b[sa->i])
		{
			sa->side[sa->i] = -1;
			sa->candidate_plane[sa->i] = sa->min_b[sa->i];
			sa->inside = false;
		}
		else if (sa->origin[sa->i] > sa->max_b[sa->i])
		{
			sa->side[sa->i] = 1;
			sa->candidate_plane[sa->i] = sa->max_b[sa->i];
			sa->inside = false;
		}
		else
			sa->side[sa->i] = 0;
		sa->i++;
	}
}

void	ric_fix_inside(t_cube_intersection_data *sa)
{
	if (sa->inside)
	{
		sa->i = 0;
		while (sa->i < 3)
		{
			if (sa->dir[sa->i] < 0)
			{
				sa->side[sa->i] = -1;
				sa->candidate_plane[sa->i] = sa->min_b[sa->i];
			}
			else if (sa->dir[sa->i] > 0)
			{
				sa->side[sa->i] = 1;
				sa->candidate_plane[sa->i] = sa->max_b[sa->i];
			}
			else
				sa->side[sa->i] = 0;
			sa->i++;
		}
	}
}

void	ric_calculate_times(t_cube_intersection_data *sa)
{
	sa->i = 0;
	while (sa->i < 3)
	{
		if (sa->side[sa->i] != 0 && sa->dir[sa->i] != 0.)
			sa->hit_t[sa->i] = (sa->candidate_plane[sa->i] - sa->origin[sa->i])
				 / sa->dir[sa->i];
		else
			sa->hit_t[sa->i] = ter_float(sa->inside, INFINITY, -INFINITY);
		sa->i++;
	}
}

void	ric_calculate_hit_plane(t_cube_intersection_data *sa)
{
	sa->which_plane = 0;
	if (sa->inside)
	{
		sa->i = 0;
		while (sa->i < 3)
		{
			if (sa->hit_t[sa->i] < sa->hit_t[sa->which_plane])
				sa->which_plane = sa->i;
			sa->i++;
		}
	}
	else
	{
		sa->i = 0;
		while (sa->i < 3)
		{
			if (sa->hit_t[sa->i] > sa->hit_t[sa->which_plane])
				sa->which_plane = sa->i;
			sa->i++;
		}
	}
}
