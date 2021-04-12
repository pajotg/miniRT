#include "stupid_norm.h"

bool	material_specular_init(t_material *material, const t_color_hdr *color,
	float pow)
{
	t_material_specular	*specular;

	specular = malloc(sizeof(t_material_specular));
	if (specular == NULL)
		return (false);
	specular->color = *color;
	specular->pow = pow;
	*material = (t_material)
	{
		.material_data = specular,
		.material_func = material_specular,
		.material_free_data_func = free
	};
	return (true);
}
