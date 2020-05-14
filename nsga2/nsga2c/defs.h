#ifndef _DEFS
#define _DEFS
#include <stddef.h>

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

typedef struct Problem {
  ObjectiveFunc *objective_funcs;
} Problem;

typedef struct NSGAIIVals {
  Problem *problem;
  Population *population;
  Population **fronts;
  double mutstren;      /* mutation strength */
  long ngen;            /* num of generation */
  size_t ninds;         /* num of individuals */
  size_t nobjs;         /* num of objectives */
  size_t nfeatures;     /* num of parameters for objectives functions */
  int ngenes_to_mutate; /* how many generations for mutation to happen */
  int ntour_particips;  /* num of tournament participants */
} NSGAIIVals;

// utils.c
void calobjs(NSGAIIVals *, double *features, double *objs);
Individual *create_emptyind();
#endif /* ifndef _DEFS */
