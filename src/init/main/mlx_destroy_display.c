#include "mlx.h"

// ROFL, THIS IS HORRENDOUS
#ifdef OS_Linux
void	mlx_destroy_display_but_only_on_linux(void *ptr)
{
	mlx_destroy_display(ptr);
}
#else

void	mlx_destroy_display_but_only_on_linux(void *ptr)
{
	(void)ptr;
}
#endif