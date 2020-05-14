#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// return 0 if error happened.
size_t init_population(NSGAIIVals *, Population *p, size_t front_sz);
Individual *create_offspring(NSGAIIVals *, Pool *p, size_t pop_sz);
void crossover(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2);
int cmp_objective(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2, int m);
void mutate(NSGAIIVals *nsga2, Individual *offspring);
void tournament(NSGAIIVals *nsga2, Population *p, Individual *best);

/* return top and bottom ptr bound elements in the rank. */
static inline void get_rank_tuple(int rank, Pool *p, Population *top,
                                  Population *bottom) {
  assert(rank <= p->nrank);
  top = p->fronts[rank + 1];
  bottom = p->fronts[rank];
}

static inline size_t get_frontsz(Population *end, Population *start) {
  return end - start;
}


#endif /* ifndef _UTILS */
