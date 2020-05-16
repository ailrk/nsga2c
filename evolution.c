#include "evolution.h"
#include "crowddist.h"
#include "fastnondominsort.h"
#include <stdlib.h>

/* setup env, allocate pool, and initialize population */
static void initpool(NSGA2ctx *nsga2, Pool *pool, Problem *problem) {
  assert(nsga2->ninds != 0);
  nsga2->problem = problem;
  allocpool(nsga2, pool);
  init_population(nsga2, pool);
  fast_nondominated_sort(nsga2, pool);
  for (size_t rank = 0; rank < pool->nrank; rank++) {
    calculate_crowd_distance(nsga2, pool, rank);
  }
}

/* generate new population based on pool->population.
 * It will allocate n->ninds * 2 elements i memory. and fill
 * n->ninds elements here. The rest of space are reserved for offspring
 * for next generation.
 * return last rank the loop hit after finishing. */
static int make_newpopulation(NSGA2ctx *nsga2, Pool *pool, Population newpop,
                              Population newpop_top) {
  assert(newpop == NULL);
  Population fbeg, fend; /* front */
  int rank = 0;
  const int  ninds = nsga2->ninds;
  size_t top = 0, frontsz = get_frontszp(rank, pool);

  /* alloc individule size to new population  */
  newpop = (Population)malloc(sizeof(Individual) * ninds * 2);

  while (top + frontsz < ninds) { /* fill half */
    calculate_crowd_distance(nsga2, pool, rank);
    /* cpy individules in front into newpop */
    get_front_tuple(rank, pool, fbeg, fend);
    for (Population p = newpop; fbeg < fend; fbeg++, p++, top++) {
      *p = *fbeg;
    }
    rank++;
  }
  newpop_top = newpop + top;
  return rank;
}

static Population newpop_end(NSGA2ctx *nsga2, Population newpop) {
  return newpop + nsga2->ninds * 2;
}

static void init_offspring(NSGA2ctx *nsga2, Pool *pool) {
  Population offset = expandpopulation(nsga2, pool, nsga2->ninds);
  create_offspring(nsga2, pool, offset);
}

/* this comparator will be used to ele in compare array of Individual*, so
 * parameter will be of type (Individual **). To be used by crowding_operator
 * they need to be derefereced. */
static int cmp_crod_dist(const void *a, const void *b) {
  return crowding_operator(*(Individual **)a, *(Individual **)b);
}

Population evolve(NSGA2ctx *nsga2, Problem *problem) {
  Pool *pool = NULL;
  Population returned_front;
  /* newpop_top: a ptr point to new_population + size of new_population */
  Population new_population = NULL, newpop_top;
  long lastfront;

  initpool(nsga2, pool, problem);
  init_offspring(nsga2, pool);
  for (int i = 0; i < nsga2->ngen; i++) {
    fast_nondominated_sort(nsga2, pool);
    lastfront = make_newpopulation(nsga2, pool, new_population, newpop_top);
    /* sort with crowding distance */
    qsort(pool->fronts[lastfront], get_frontszp(lastfront, pool),
          sizeof(Population *), cmp_crod_dist);

    /* extend newpop till span the entire size of <2 x ninds> */
    for (size_t i = 0; i < nsga2->ninds - (newpop_top - new_population); i++) {
      newpop_top[i] = pool->fronts[lastfront][i];
    }
    freepool(pool);
    pool->population = new_population;

    create_offspring(nsga2, pool, newpop_top);
  }

  return returned_front;
}
