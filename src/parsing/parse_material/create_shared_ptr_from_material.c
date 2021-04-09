#include "stupid_norm.h"

/*
**	Pass in a material that has been created with init
**	If it fails it will also free the material data
*/

t_shared_pt	*create_shared_ptr_from_material(t_material *material)
{
	t_material	*material_ptr;
	t_shared_pt	*ptr;

	material_ptr = malloc(sizeof(t_material));
	if (!material_ptr)
	{
		material->material_free_data_func(material->material_data);
		return (NULL);
	}
	*material_ptr = *material;
	ptr = shared_pt_new(material_ptr, (t_free_func)material_free);
	if (!ptr)
		material_free(material_ptr);
	return (ptr);
}
