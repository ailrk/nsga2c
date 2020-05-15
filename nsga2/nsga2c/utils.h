#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// return 0 if error happened.
int allocpopulation(NSGAIIVals *, Pool *);
void freepopulation(Pool *);
void rellocpopulation(NSGAIIVals *nsga2, Pool *p, size_t n);

int allocfronts(Pool *, size_t rank);
void freefronts(Pool *);

void init_population(NSGAIIVals *, Pool *p);
void create_offspring(NSGAIIVals *, Pool *p, Population offset);

// operators
void crossover(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2);
void mutate(NSGAIIVals *nsga2, Individual *offspring);
void tournament(NSGAIIVals *nsga2, Population *p, Individual *best);

/* return top and bottom ptr bound elements in the rank. */
static inline void get_rank_tuple(size_t rank, Pool *p, Population top,
                                  Population bottom) {
  assert(rank <= p->nrank);
  top = p->fronts[rank + 1];
  bottom = p->fronts[rank];
}

static inline size_t get_frontsz(Population *end, Population *start) {
  return end - start;
}

#endif /* ifndef _UTILS */
