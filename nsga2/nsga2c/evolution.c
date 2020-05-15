#include "evolution.h"
#include "crowddist.h"
#include "fastnondominsort.h"
#include <stdlib.h>

static void init(NSGAIIVals *nsga2, Pool *pool, Problem *problem) {
  nsga2->problem = problem;
  pool = (Pool *)malloc(sizeof(Pool));
  init_population(nsga2, pool);
  fast_nondominated_sort(nsga2, pool);
  for (size_t rank = 0; rank < pool->nrank; rank++) {
    calculate_crowd_distance(nsga2, pool, rank);
  }
}

Population evolve(NSGAIIVals *nsga2, Problem *problem) {
  Pool *pool = NULL;
  Population returned_pop;
  Population offspring;
  long frontnum;
  init(nsga2, pool, problem);
  offspring = create_offspring(nsga2, pool);
  for (int i = 0; i < nsga2->ngen; i++) {
    /* extend population with children */
    update_pool(nsga2, pool, offspring, nsga2->ninds);
    fast_nondominated_sort(nsga2, pool);
    frontnum = 0;
    while () {
    }
  }


  free(pool->population);
  free(pool->fronts);
  free(pool);
}
