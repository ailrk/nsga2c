#ifndef _CROWDDIST
#define _CROWDDIST
#include "defs.h"

void calculate_crowd_distance(NSGAIIVals *, Pool *p, int rank);
bool crowding_operator(Individual *self, Individual *other);

#endif /* ifndef _CROWDDIST */