#include "evolution.h"
#include "crowddist.h"
#include "fastnondominsort.h"
#include <stdlib.h>

/* setup env, allocate pool, and initialize population */
static void initpool(NSGAIIVals *nsga2, Pool *pool, Problem *problem) {
  nsga2->problem = problem;
  allocpool(nsga2, pool);
  init_population(nsga2, pool);
  fast_nondominated_sort(nsga2, pool);
  for (size_t rank = 0; rank < pool->nrank; rank++) {
    calculate_crowd_distance(nsga2, pool, rank);
  }
}

/* generate new population based on pool->population.
 * return last rank the loop hit after finishing. */
static int make_newpopulation(NSGAIIVals *nsga2, Pool *pool, Population newpop) {
  assert(newpop == NULL);
  Population beg, end;
  int rank = 0;
  size_t newpop_top = 0, frontsz = get_frontszp(rank, pool);

  /* alloc individule size to new population  */
  newpop = (Population)malloc(sizeof(Individual) * nsga2->ninds * 2);

  while (newpop_top + frontsz < nsga2->ninds) {
    calculate_crowd_distance(nsga2, pool, rank);
    /* cpy individules in front into newpop */
    get_rank_tuple(rank, pool, beg, end);
    for (Population b = beg, p = newpop; b < end; b++, p++, newpop_top++) {
      *p = *b;
    }
    rank++;
  }
  return rank;
}

Population evolve(NSGAIIVals *nsga2, Problem *problem) {
  Pool *pool = NULL;
  Population returned_front, offset, new_pop = NULL;
  long nfronts;
  initpool(nsga2, pool, problem);

  offset = expandpopulation(nsga2, pool, nsga2->ninds);
  create_offspring(nsga2, pool, offset);
  for (int i = 0; i < nsga2->ngen; i++) {
    fast_nondominated_sort(nsga2, pool);
    nfronts = make_newpopulation(nsga2, pool, new_pop);
    // sort with crowding distance
    qsort(pool->fronts[nfronts],  crowding_operator)
    // extend
  }

  return returned_front;
}
