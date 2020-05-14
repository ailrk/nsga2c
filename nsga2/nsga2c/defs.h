#ifndef _DEFS
#define _DEFS
#include <stddef.h>
#include <stdlib.h>

typedef struct Individual {
  int rank;                      /* rank based on nondetermination */
  double crowd_dist;             /* corwding distance */
  double *features;              /* parameters of objectives function */
  double *objs;                  /* result of objective functions  */
  struct Individual **dominates; /* list of individules it dominates */
  long ndomin;                   /* number of individules dominated it. */
} Individual;

#define EMPTY_INDIVIDUAL                                                       \
  {                                                                            \
    .rank = 0, .crowding_dist = 0.0, features = NULL, objs = NULL,             \
    dominated = NULL, ndominated = 0L                                          \
  }

typedef Individual *Population;
typedef double (*ObjectiveFunc)(double *features);

typedef struct Tuple {
  double upper;
  double lower;
} Tuple;

typedef struct Problem {
  ObjectiveFunc *objective_funcs;
  Tuple *feature_domains; /* tuple of feature upper and lower bound */
} Problem;

typedef struct NSGAIIVals {
  Problem *problem;
  double mutstren;      /* mutation strength */
  long ngen;            /* num of generation */
  size_t ninds;         /* num of individuals */
  size_t nobjs;         /* num of objectives */
  size_t nfeatures;     /* num of parameters for objectives functions */
  int ngenes_to_mutate; /* how many generations for mutation to happen */
  int ntour_particips;  /* num of tournament participants */
} NSGAIIVals;

typedef struct Pool {
  Population *population; /* all population in pool */
  Population **fronts;    /* fronts in population. stores ptr to population. */
  size_t nrealpop;        /* real number of population */
  size_t nrank;           /* total number of fronts */
} Pool;

/* helpers */

/* calculate objectives some features. Put result in double *objs */
static inline void calobjs(NSGAIIVals *nsga2, double *features, double *objs) {
  for (size_t i = 0; i < nsga2->nobjs; i++) {
    objs[i] = nsga2->problem->objective_funcs[i](features);
  }
}

static inline bool dominates(NSGAIIVals *nsga2, Individual *a, Individual *b) {
  bool result = true;
  for (size_t i = 0; i < nsga2->nobjs; i++) {
    if (a->objs[i] < b->objs[i])
      result = false;
  }
  return result;
}

static inline Individual *create_emptyind() {
  Individual *p = (Individual *)malloc(sizeof(Individual));
  *p = EMPTY_INDIVIDUAL;
  return p;
}

// utils.c
bool update_pool(NSGAIIVals *, Pool *p, Individual *other, size_t other_n);

#endif /* ifndef _DEFS */
