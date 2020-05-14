#ifndef _CROWDDIST
#define _CROWDDIST
#include "defs.h"

void get_crowd_distance(NSGAIIVals *, Pool *p, size_t front_sz);
bool crowding_operator(NSGAIIVals *, Individual *self, Individual *other);

#endif /* ifndef _CROWDDIST */
