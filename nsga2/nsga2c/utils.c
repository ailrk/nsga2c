#include "utils.h"
#include "crowddist.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* private operators */
static void pickrand(int *buffer, size_t n, int *random);
static void swap(double *a, double *b);

/* create initial population with random features and objectives calculated.
 * return 0 on error */
void init_population(NSGAIIVals *nsga2, Pool *p) {
  double upper, lower, feature;
  for (int i = 0; i < nsga2->ninds; i++) {
    p->population[i] = create_emptyind();
    for (int j = 0; j < nsga2->nfeatures; j++) {
      /* generate random feature */
      upper = nsga2->problem->feature_domains[i].upper;
      lower = nsga2->problem->feature_domains[i].lower;
      feature = fmod(rand(), (upper - lower + 1) + lower);
      p->population[i]->features[j] = feature;
    }
    /* calculate objective */
    calobjs(nsga2, p->population[i]->features, p->population[i]->objs);
  }
}

/* create children from current population */
Population create_offspring(NSGAIIVals *nsga2, Pool *p) {
  Individual *children = (Individual *)malloc(sizeof(Individual) * nsga2->ninds);
  /* make sure has 2 empty space */
  for (int i = 0; i < nsga2->ninds; i += 2) {
    Individual *parent1 = NULL, *parent2, *child1, *child2;
    tournament(nsga2, p->population, parent1);
    parent2 = parent1;
    /* make sure parent1 and parent2 are different individuals */
    while (parent2 == parent1) {
      tournament(nsga2, p->population, parent2);
    }
    *child1 = *parent1;
    *child2 = *parent2;
    mutate(nsga2, child1);
    mutate(nsga2, child2);
    calobjs(nsga2, child1->features, child1->objs);
    calobjs(nsga2, child2->features, child2->objs);

    children[i] = *child1;
    children[i + 1] = *child2;
  }
  return children;
}

/* stitch new Population to pool and update nrealpop */
bool update_pool(NSGAIIVals *nsga2, Pool *p, Individual *other,
                 size_t other_n) {
  Individual *ptr;
  if (other_n <= 0) {
    errno = ERANGE;
    perror("error happend when updating population");
    return false;
  }
  *p->population = (Population)realloc(p->population, p->nrealpop + other_n);
  ptr = *p->population + other_n;
  *ptr = *other;
  free(other);
  /* update size information */
  p->nrealpop += other_n;
  return true;
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
void crossover(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2) {
  int random, n = nsga2->nfeatures;
  FOR_EACH_RANDOM(random, n)
  swap(&ind1->features[random], &ind2->features[random]);
  END_FOR_EACH_RANDOM
}

void mutate(NSGAIIVals *nsga2, Individual *offspring) {
  int random, n = nsga2->nfeatures;
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
void tournament(NSGAIIVals *nsga2, Population *p, Individual *best) {
  int random, n = nsga2->ntour_particips;
  FOR_EACH_RANDOM(random, n)
  Individual *participant = p[random];
  if (best == NULL || crowding_operator(nsga2, participant, best) == true) {
    best = participant;
  }
  END_FOR_EACH_RANDOM
}

static void pickrand(int *buffer, size_t n, int *random) {
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

