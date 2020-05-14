#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <stdbool.h>
#include <stdlib.h>

// return 0 if error happened.
size_t init_population(NSGAIIVals *, Population *p, size_t front_sz);
Individual *create_offspring(NSGAIIVals *, Pool *p, size_t pop_sz);
void crossover(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2);
int cmp_objective(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2, int m);
void mutate(NSGAIIVals *nsga2, Individual *offspring);
void tournament(NSGAIIVals *nsga2, Population *p, Individual *best);
#endif /* ifndef _UTILS */
