#include "fastnondominsort.h"
#include "utils.h"
#include "defs.h"
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>

static void swapind(Individual *a, Individual *b) {
  Individual temp = *a;
  *a = *b;
  *b = temp;
}

/* update rank my 1, and point the new front to top */
static bool update_rank(Pool *p, int rank, Population top) {
  /* realloc if space is not enough */
  if (rank >= p->fronts_sz) {
    p->fronts_sz = (int)(p->fronts_sz * 1.6);
    p->fronts =
        (Population *)realloc(p->fronts, sizeof(Population) * p->fronts_sz);
  }
  if (rank - 1 == p->nrank && rank < p->fronts_sz) {
    p->fronts[rank] = top;
    return true;
  }
  return false;
}

/* tag the domination information for an individule. rank 0 will be sorted. */
static void tag_dominations(NSGA2ctx *nsga2, Pool *p) {
  const int n = p->nrealpop;
  Individual *ind, *other;
  // TODO
  Population sp = p->population;
  for (size_t i = 0; i < n; i++) {
    ind = &p->population[i];
    ind->ndomin = 0;

    /* use linked list because the contain needs to be update dyamically frequently
     * ind->dominates will only be traversaled, linked list is OK. */
    ind->dominates = linew_head();
    for (int j = 0; j < p->nrealpop; j++) {
      other = &p->population[j];
      if (dominates(nsga2, ind, other)) {
        lipush(ind->dominates, other);
      } else if (dominates(nsga2, other, ind)) {
        ind->ndomin += 1;
      }
    }
    if (ind->ndomin == 0) {

      /* swap to the front. */
      swapind(sp++, ind);
      ind->rank = 0;
    }
  }

  /* end of the rank 0 in rank buf */
  update_rank(p, 1, sp);
}

/* keep decrementing ndomin of individul until it hit 0, assign a rank for it
 * base on the number of iteration */
static void assign_rank(NSGA2ctx *nsga2, Pool *p) {
  size_t last_ranksz;
  Population front_end = NULL, front_beg = NULL;
  Individual *ind = NULL, *other = NULL;
  const IndList *dominates;

  /* at this point rank 0 should already be sorted. */
  assert(p->nrank == 1);
  get_front_tuple(0, p, front_end, front_beg);
  last_ranksz = get_frontsz(front_beg, front_end);

  while (last_ranksz > 0) {
    for (int i = 0; i < last_ranksz; i++) {
      ind = &front_beg[i];
      dominates = ind->dominates;
      other = dominates->ind;

      /* traversal the linked list */
      while (other != NULL) {
        other->ndomin -= 1;
        if (other->ndomin == 0) {
          other->rank += 1;

          /* move other to the front */
          swapind(front_end++, other);
        }
        dominates = dominates->next;
        other = dominates->ind;
      }
    }
    p->nrank++;
    update_rank(p, p->nrank, front_end);
    get_front_tuple(p->nrank, p, front_end, front_beg);
    last_ranksz = get_frontsz(front_end, front_beg);
  }
}

/* population should be 2x the size of original ninds in NSGAIIVals
 * Because of the newly appeneded offspring */
bool fast_nondominated_sort(NSGA2ctx *nsga2, Pool *p) {
  /* new children should be appened at this point */
  if (p->nrealpop <= nsga2->ninds) {
    errno = ERANGE;
    perror("population under size during fast non determination sort.");
    return false;
  }
  p->fronts_sz = 100;

  /* p->fronts stores ptr to different part of population. (like partioning)
   * all individules can be fit into population.
   * NOTE: worst case is a single rank with all individules,
   * in wich case p->fronts[0] (rank 0) has the same size as frontbuf. */
  p->fronts = (Population *)malloc(sizeof(Population) * p->fronts_sz);
  tag_dominations(nsga2, p);
  assign_rank(nsga2, p);
  return true;
}
