#ifndef _CROWDDIST
#define _CROWDDIST
#include "defs.h"

void get_crowd_distance(NSGAIIVals *, Pool *p, int rank);
bool crowding_operator(NSGAIIVals *, Individual *self, Individual *other);

#endif /* ifndef _CROWDDIST */
