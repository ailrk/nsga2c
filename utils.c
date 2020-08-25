#include "utils.h"
#include "crowddist.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* pick random number */

static void pickrand(int *buffer, const size_t n, int *random) {
  do
    *random = rand() % n;
  while (buffer[*random] != -1);
  buffer[*random] = 1;
}

static void swap(double *a, double *b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

/* mem */

int allocpopulation(const NSGA2ctx *nsga2, Pool *p) {
  p->population = (Population)malloc(sizeof(Individual) * nsga2->ninds);
  if (p->population == NULL) {
    perror("error when allocating population");
    return -1;
  }
  p->nrealpop = nsga2->ninds;
  return 0;
}

void freepopulation(Pool *p) {
  free(p->population);
  p->nrealpop = 0;
}

/* return the begining of the newly allocated memory */
Population expandpopulation(NSGA2ctx *nsga2, Pool *p, const size_t delta) {
  if (delta <= 0) {
    return NULL;
  }
  const Population offset = p->population + p->nrealpop;
  const size_t newsz = p->nrealpop + delta;
  p->population =
      (Population)realloc(p->population, sizeof(Individual) * newsz);
  p->nrealpop = newsz;
  return offset;
}

int allocfronts(Pool *p, const size_t rank) {
  if (p->fronts == NULL) {
    return -1;
  }
  p->fronts = (Population *)malloc(sizeof(Population) * rank);
  if (p->fronts == NULL) {
    perror("failed to allocate fronts");
    return -1;
  }
  p->nrank = rank;
  return 0;
}

void freefronts(Pool *pool) {
  free(pool->fronts);
  pool->nrank = 0;
}

int allocpool(NSGA2ctx *nsga2, Pool *pool) {
  if (pool->population != NULL) {
    return 1;
  }
  allocpopulation(nsga2, pool);
  return 0;
}

void freepool(Pool *pool) {
  freefronts(pool);
  freepopulation(pool);
}

/* create initial population with random features and objectives calculated.
 * return 0 on error */
void init_population(NSGA2ctx *nsga2, Pool *p) {
  assert(p->population != NULL);
  double upper, lower, feature;
  for (int i = 0; i < nsga2->ninds; i++) {
    mkind(&p->population[i]);
    for (int j = 0; j < nsga2->nfeatures; j++) {
      /* generate random feature */
      upper = nsga2->problem->feature_domains[i].upper;
      lower = nsga2->problem->feature_domains[i].lower;
      feature = fmod(rand(), (upper - lower + 1) + lower);
      p->population[i].features[j] = feature;
    }
    /* calculate objective */
    calobjs(nsga2, p->population[i].features, p->population[i].objs);
  }
}

/* create children from current population
 * space should be already allocated. */
void create_offspring(NSGA2ctx *nsga2, Pool *p, const Population offset) {
  assert(p->nrealpop <= nsga2->ninds * 2);

  /* extend population to fit offsprings */
  assert(offset != NULL);

  /* make sure has 2 empty space */
  for (int i = 0; i < nsga2->ninds; i += 2) {
    Individual *parent1 = NULL, *parent2, *child1, *child2;
    tournament(nsga2, &p->population, parent1);
    parent2 = parent1;

    /* make sure parent1 and parent2 are different individuals */
    while (parent2 == parent1) {
      tournament(nsga2, &p->population, parent2);
    }
    *child1 = *parent1;
    *child2 = *parent2;
    mutate(nsga2, child1);
    mutate(nsga2, child2);
    calobjs(nsga2, child1->features, child1->objs);
    calobjs(nsga2, child2->features, child2->objs);

    offset[i] = *child1;
    offset[i + 1] = *child2;
  }
}

#define FOR_EACH_RANDOM(random, n)                                             \
  srand((int)time(NULL));                                                      \
  int buffer[n];                                                               \
  memset(buffer, -1, n);                                                       \
  for (int i = 0; i < n / 2; i++) {                                            \
    pickrand(buffer, n, &random);                                              \
    assert(random <= n);
#define END_FOR_EACH_RANDOM }

/* swith half randomly selected features between two individuls */
void crossover(NSGA2ctx *nsga2, Individual *ind1, Individual *ind2) {
  int random;
  const int n = nsga2->nfeatures;
  FOR_EACH_RANDOM(random, n)
  swap(&ind1->features[random], &ind2->features[random]);
  END_FOR_EACH_RANDOM
}

void mutate(NSGA2ctx *nsga2, const Individual *offspring) {
  int random;
  const int n = nsga2->nfeatures;
  FOR_EACH_RANDOM(random, n)
  offspring->features[random] = offspring->features[random] -
                                nsga2->mutstren / 2 +
                                (rand() % 100) / 1000 * nsga2->mutstren;
  if (offspring->features[random] < 0) {
    offspring->features[random] = 0;
  } else if (offspring->features[random] > 1) {
    offspring->features[random] = 1;
  }
  END_FOR_EACH_RANDOM
}

/* do tournament and set best to the winner.
 * best can be NULL. */
void tournament(NSGA2ctx *nsga2, Population *p, Individual *best) {
  int random;
  const int n = nsga2->ntour_particips;
  FOR_EACH_RANDOM(random, n)
  Individual *participant = p[random];
  if (best == NULL || crowding_operator(participant, best) == 1) {
    best = participant;
  }
  END_FOR_EACH_RANDOM
}
