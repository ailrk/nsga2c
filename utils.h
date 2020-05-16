#ifndef _UTILS
#define _UTILS
#include "defs.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// return 0 if error happened.
int allocpopulation(const NSGA2ctx *, Pool *);
void freepopulation(Pool *);
Population expandpopulation(NSGA2ctx *nsga2, Pool *p, const size_t n);

int allocfronts(Pool *, const size_t rank);
void freefronts(Pool *);

int allocpool(NSGA2ctx *, Pool *);
void freepool(Pool *);

void init_population(NSGA2ctx *, Pool *p);
void create_offspring(NSGA2ctx *, Pool *p, const Population offset);

// operators
void crossover(NSGA2ctx *nsga2, Individual *ind1, Individual *ind2);
void mutate(NSGA2ctx *nsga2, const Individual *offspring);
void tournament(NSGA2ctx *nsga2, Population *p, Individual *best);

/* return top and bottom ptr bound elements in the rank. */
static inline void get_front_tuple(const size_t rank, const Pool *p,
                                   Population top, Population bottom) {
  assert(rank <= p->nrank);
  top = p->fronts[rank + 1];
  bottom = p->fronts[rank];
}

static inline size_t get_frontsz(const Population start, const Population end) {
  return end - start;
}

static inline size_t get_frontszp(const size_t rank, const Pool *p) {
  Population start = NULL, end = NULL;
  get_front_tuple(rank, p, start, end);
  return get_frontsz(start, end);
}

#endif /* ifndef _UTILS */
