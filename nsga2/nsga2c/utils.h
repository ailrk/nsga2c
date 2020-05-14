#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <stdbool.h>
#include <stdlib.h>

bool fast_nondominated_sort(NSGAIIVals *, Pool *);
void get_crowd_distance(NSGAIIVals *, Population *front, size_t front_sz);
bool crowding_operator(NSGAIIVals *, Individual *self, Individual *other);
// return 0 if error happened.
size_t init_population(NSGAIIVals *, Population *p, size_t front_sz);
Individual *create_offspring(NSGAIIVals *, Pool *p, size_t pop_sz);
#endif /* ifndef _UTILS */
