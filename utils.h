#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// return 0 if error happened.
int allocpopulation(NSGA2ctx *, Pool *);
void freepopulation(Pool *);
Population expandpopulation(NSGA2ctx *nsga2, Pool *p, size_t n);

int allocfronts(Pool *, size_t rank);
void freefronts(Pool *);

int allocpool(NSGA2ctx *, Pool *);
void freepool(Pool *);

void init_population(NSGA2ctx *, Pool *p);
void create_offspring(NSGA2ctx *, Pool *p, Population offset);

// operators
void crossover(NSGA2ctx *nsga2, Individual *ind1, Individual *ind2);
void mutate(NSGA2ctx *nsga2, Individual *offspring);
void tournament(NSGA2ctx *nsga2, Population *p, Individual *best);

/* return top and bottom ptr bound elements in the rank. */
static inline void get_front_tuple(size_t rank, Pool *p, Population top,
                                  Population bottom) {
  assert(rank <= p->nrank);
  top = p->fronts[rank + 1];
  bottom = p->fronts[rank];
}

static inline size_t get_frontsz(Population start, Population end) {
  return end - start;
}

static inline size_t get_frontszp(size_t rank, Pool *p) {
  Population start, end;
  get_front_tuple(rank, p, start, end);
  return get_frontsz(start, end);
}

#endif /* ifndef _UTILS */
