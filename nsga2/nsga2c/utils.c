#include "utils.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* private operators */
static int cmp_objective(NSGAIIVals *, Individual *ind1, Individual *ind2,
                         int m);
static void crossover(NSGAIIVals *, Individual *ind1, Individual *ind2);
static void mutate(NSGAIIVals *, Individual *offspring);
static void tournament(NSGAIIVals *, Population *p, Individual *best);
static void pickrand(int *buffer, size_t n, int *random);
static void swap(double *a, double *b);

/* create initial population with random features and objectives calculated.
 * return 0 on error */
size_t init_population(NSGAIIVals *nsga2, Population *p, size_t front_sz) {
  if (nsga2->ninds != front_sz)
    return 0;

  double upper, lower, feature;
  for (int i = 0; i < front_sz; i++) {
    p[i] = create_emptyind();
    for (int j = 0; j < nsga2->nfeatures; j++) {
      /* generate random feature */
      upper = nsga2->problem->feature_domains[i].upper;
      lower = nsga2->problem->feature_domains[i].lower;
      feature = fmod(rand(), (upper - lower + 1) + lower);
      p[i]->features[j] = feature;
    }
    /* calculate objective */
    calobjs(nsga2, p[i]->features, p[i]->objs);
  }

  return front_sz;
}

/* create children from current population */
Individual *create_offspring(NSGAIIVals *nsga2, Pool *p, size_t pop_sz) {
  Individual *children = (Individual *)malloc(sizeof(Individual) * pop_sz);
  int i = 0;
  /* make sure has 2 empty space */
  for (int i = 0; i < pop_sz; i += 2) {
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

/* update rank my 1, and point the new front to top */
static bool update_rank(Pool *p, int rank, Population *top) {
  if (rank - 1 == p->nrank) {
    p->fronts = (Population **)realloc(p->fronts, sizeof(Population **) * rank);
    p->fronts[rank] = top;
    return true;
  }
  return false;
}

/* tag the domination information for an individule. rank 0 will be sorted. */
static void tag_dominations(NSGAIIVals *nsga2, Pool *p, Population *rankbuf) {
  int n = p->nrealpop;
  Individual *ind, *other;
  Population *sp = rankbuf;
  for (size_t i = 0; i < n; i++) {
    ind = *p->population + i;
    ind->ndomin = 0;
    ind->dominates = NULL;
    for (int j = 0; j < p->nrealpop; j++) {
      other = *p->population + j;
      if (dominates(nsga2, ind, other)) {
        // add other to dominates
      } else if (dominates(nsga2, other, ind)) {
        ind->ndomin += 1;
      }
    }
    if (ind->ndomin == 0) {
      *sp++ = ind;
      ind->rank = 0;
    }
  }
  /* end of the rank 0 in rank buf */
  update_rank(p, 1, sp);
}

static void assign_rank(NSGAIIVals *nsga2, Pool *p, Population *rankbuf) {
  int rank = 0, ranksz = 0;
  Population *sp = rankbuf;
  const size_t n = p->nrealpop;

  while (ranksz > 0) {
    //
  }

}

/* population should be 2x the size of original ninds in NSGAIIVals
 * Because of the newly appeneded offspring */
bool fast_nondominated_sort(NSGAIIVals *nsga2, Pool *p) {
  const size_t n = p->nrealpop;
  Population *rankbuf;
  /* new children should be appened at this point */
  if (p->nrealpop <= nsga2->ninds) {
    errno = ERANGE;
    perror("population under size during fast non determination sort.");
    return false;
  }
  /* p->fronts stores ptr to different part of rankbuf.
   * all population can be fit into * rankbuf.
   * worst case is a single rank with all individules,
   * in wich case p->fronts[0] (rank 0) has the same size as frontbuf. */
  p->fronts = (Population **)malloc(sizeof(Population **));
  rankbuf = (Population *)malloc(sizeof(Individual *) * n);
  tag_dominations(nsga2, p, rankbuf);
  assign_rank(nsga2, p, rankbuf);
  return true;
}

void get_crowd_distance(NSGAIIVals *nsga2, Population *front, size_t front_sz) {

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
