#ifndef _CROWDDIST
#define _CROWDDIST
#include "defs.h"

void calculate_crowd_distance(NSGA2ctx *, Pool *p, const int rank);
int crowding_operator(Individual *self, Individual *other);

#endif /* ifndef _CROWDDIST */
