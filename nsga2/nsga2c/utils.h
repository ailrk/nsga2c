#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// return 0 if error happened.
void init_population(NSGAIIVals *, Pool *p);
Population create_offspring(NSGAIIVals *, Pool *p);
void crossover(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2);
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

static inline void exend_population_with_offspring(NSGAIIVals *nsga2, Pool *p) {
  Population *end;
  end = p->population + p->nrealpop;
  p->population = (Population *)realloc(
      p->population, sizeof(Population) * (nsga2->ninds + p->nrealpop));

}

#endif /* ifndef _UTILS */
