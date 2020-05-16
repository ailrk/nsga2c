#include "evolution.h"
#include "crowddist.h"
#include "fastnondominsort.h"
#include <stdlib.h>

static void init(NSGAIIVals *nsga2, Pool *pool, Problem *problem) {
  nsga2->problem = problem;
  allocpool(nsga2, pool);
  init_population(nsga2, pool);
  fast_nondominated_sort(nsga2, pool);
  for (size_t rank = 0; rank < pool->nrank; rank++) {
    calculate_crowd_distance(nsga2, pool, rank);
  }
}

/* return front number */
static int mk_newpop(NSGAIIVals *nsga2, Pool *pool, Population newpop) {
  assert(newpop == NULL);
  int rank = 0;
  size_t newpop_top = 0, frontsz = get_frontszp(rank, pool);

  /* alloc individule size to new population  */
  newpop = (Population)malloc(sizeof(Individual) * nsga2->ninds);

  while (newpop_top + frontsz < nsga2->ninds) {
    calculate_crowd_distance(nsga2, pool, rank);
    /* cpy front into newpop */
    newpop[newpop_top] =
    rank++;
  }

}

Population evolve(NSGAIIVals *nsga2, Problem *problem) {
  Pool *pool = NULL;
  Population returned_pop, offspring, new_pop;
  long frontnum;

  init(nsga2, pool, problem);
  create_offspring(nsga2, pool);
  for (int i = 0; i < nsga2->ngen; i++) {
    fast_nondominated_sort(nsga2, pool);
    frontnum = mk_newpop(nsga2, pool, new_pop, )
  }
}
