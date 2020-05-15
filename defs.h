#ifndef _DEFS
#define _DEFS
#include <stddef.h>
#include <stdlib.h>

typedef struct IndList IndList;

// Individual is actually a linked list.
typedef struct Individual {
  int rank;           /* rank based on nondetermination */
  double crowd_dist;  /* corwding distance */
  double *features;   /* parameters of objectives function */
  double *objs;       /* result of objective functions  */
  IndList *dominates; /* list of individules it dominates */
  long ndomin;        /* number of individules dominated it. */
} Individual;

typedef struct IndList {
  Individual *ind;
  struct IndList *next;
} IndList;

static inline IndList *linew_head() {
  IndList *list = (IndList *)malloc(sizeof(IndList));
  *list = {.ind = NULL, .next = NULL};
  return list;
}

static inline void lipush(IndList *list, Individual *ind) {
  list->ind = ind;
  list->next = NULL;
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
  size_t ninds;         /* num of individuals nominated */
  size_t nobjs;         /* num of objectives */
  size_t nfeatures;     /* num of parameters for objectives functions */
  double maxobj;        /* max objectives */
  double minobj;        /* min objectives */
  int ngenes_to_mutate; /* how many generations for mutation to happen */
  int ntour_particips;  /* num of tournament participants */
} NSGAIIVals;

typedef struct Pool {
  Population population; /* all population in pool */
  Population *fronts;    /* fronts in population. stores ptr to population. */
  size_t nrealpop;       /* real number of population. */
  size_t nrank;          /* current number of fronts. */
  size_t fronts_sz;      /* the true size allocated for fronts */
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

static inline void mkind(Individual *ind) {
  *ind = {.rank = 0,
          .crowd_dist = 0.0,
          .features = NULL,
          .objs = NULL,
          .dominates = NULL,
          .ndomin = 0L};
}

#endif /* ifndef _DEFS */
