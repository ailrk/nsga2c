#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <stdbool.h>
#include <stdlib.h>

void fast_nondominated_sort(NSGAIIVals *, Population *);
void get_crowd_distance(NSGAIIVals *, Population *front, size_t front_sz);
bool crowding_operator(NSGAIIVals *, Individual *self, Individual *other);
// return 0 if error happened.
size_t init_population(NSGAIIVals *, Population *p, size_t front_sz);
size_t create_offspring(NSGAIIVals *, Population *p, size_t pop_sz);
#endif /* ifndef _UTILS */
