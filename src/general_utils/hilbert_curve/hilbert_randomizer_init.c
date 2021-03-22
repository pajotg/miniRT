#include "mini_rt_hilbert_randomizer.h"

void	hilbert_randomizer_init(t_hilbert_randomizer *curve)
{
	hilbert_curve_init(&curve->curve, 0);
	curve->index = 0;
	curve->stage = 0;
}

void	hilbert_randomizer_init_custom(t_hilbert_randomizer *curve, int index,
	int stage, int recursion)
{
	hilbert_curve_init(&curve->curve, recursion);
	curve->index = index;
	curve->stage = stage;
}
