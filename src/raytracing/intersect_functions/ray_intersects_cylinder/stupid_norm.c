#include "stupid_norm.h"
#include "ft_ternary.h"

bool	ric_ray_intersects_cylinder_top_and_bottom(t_cylinder_caps_args cca)
{
	t_cylinder_caps_data	ccd;

	ccd.Above = (cca.dir->z < 0) != (cca.origin->z < cca.Height && cca.origin
			->z > -cca.Height);
	ccd.Plane_z = ter_float(ccd.Above, cca.Height, -cca.Height);
	ccd.Dist = (ccd.Plane_z - cca.origin->z) / cca.dir->z;
	if (ccd.Dist > cca.o_hit->distance)
		return (false);
	ccd.OutX = cca.origin->x + cca.dir->x * ccd.Dist;
	ccd.OutY = cca.origin->y + cca.dir->y * ccd.Dist;
	ccd.SqrMag = ccd.OutX * ccd.OutX + ccd.OutY * ccd.OutY;
	if (ccd.SqrMag > cca.Radius * cca.Radius)
		return (false);
	cca.o_hit->distance = ccd.Dist;
	vec3_scale(&cca.o_hit->location, cca.dir, ccd.Dist);
	vec3_add(&cca.o_hit->location, &cca.o_hit->location, cca.origin);
	cca.o_hit->normal = (t_vec3){0, 0, ter_float(cca.dir->z > 0, -1, 1)};
	return (true);
}

void	ric_calculate_local_space(t_cylinder_intersection_data *cid,
	const t_object *object, const t_ray *ray)
{
	cid->data = object->object_data;
	quaternion_conjugate(&cid->conj, &object->transform.rotation);
	vec3_subtract(&cid->origin, &ray->origin, &object->transform.position);
	quaternion_mult_vec3(&cid->origin, &cid->conj, &cid->origin);
	quaternion_mult_vec3(&cid->dir, &cid->conj, &ray->direction);
}
