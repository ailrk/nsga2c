#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/***********************
 *  private operators  *
 ***********************/
static int cmp_objective(NSGAIIVals *, Individual *ind1, Individual *ind2,
                         int m);
static void crossover(NSGAIIVals *, Individual *ind1, Individual *ind2);
static void mutate(NSGAIIVals *, Individual *offspring);
static void tournament(NSGAIIVals *, Population *p, Individual *best);
static void pickrand(int *buffer, size_t n, int *random);
static void swap(double *a, double *b);

/* calculate objectives with same features */
void calobjs(NSGAIIVals *nsga2, double *features, double *objs) {
  int nobj = nsga2->nobjs;
  Problem *problem = nsga2->problem;
  for (int i = 0; i < nobj; i++) {
    objs[i] = problem->objective_funcs[i](features);
  }
}

Individual *create_emptyind() {
  Individual *p = (Individual *)malloc(sizeof(Individual));
  *p = EMPTY_INDIVIDUAL;
  return p;
}

/* create initial random population. return 0 on error */
size_t init_population(NSGAIIVals *nsga2, Population *p, size_t front_sz) {
  if (nsga2->ninds != front_sz)
    return 0;
  for (int i = 0; i < nsga2->ninds; i++) {
  }
}

void fast_nondominated_sort(NSGAIIVals *nsga2, Population *p) {}

void get_crowd_distance(NSGAIIVals *nsga2, Population *front,
                           size_t front_sz) {}
/* It will be used to sort individuals before calcualte crowing distance */
static int cmp_objective(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2,
                         int m) {
  if (m < 0 || m > nsga2->nobjs) {
    perror("Accessing non exsited objective");
  }
  int o1 = ind1->objs[m];
  int o2 = ind2->objs[m];
  return o1 == o2 ? 0 : (o1 > o2 ? 1 : -1);
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
static void crossover(NSGAIIVals *nsga2, Individual *ind1, Individual *ind2) {
  int random, n = nsga2->nfeatures;
  FOR_EACH_RANDOM(random, n)
  swap(&ind1->features[random], &ind2->features[random]);
  END_FOR_EACH_RANDOM
}

static void mutate(NSGAIIVals *nsga2, Individual *offspring) {
  int random, n = nsga2->nfeatures;
  FOR_EACH_RANDOM(random, n)
  offspring->features[random] =
      offspring->features[random] - nsga2->mutstren / 2 +
      (rand() % 100) / 1000 * nsga2->mutstren;
  if (offspring->features[random] < 0) {
    offspring->features[random] = 0;
  } else if (offspring->features[random] > 1) {
    offspring->features[random] = 1;
  }
  END_FOR_EACH_RANDOM
}

/* do tournament and set best to the winner. */
static void tournament(NSGAIIVals *nsga2, Population *p, Individual *best) {
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

