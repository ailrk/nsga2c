#include "crowddist.h"
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

/* index of objective in an Individual */

static int cmp_objective(const void *a, const void *b, void *idx) {
  Individual *self = (Individual *) a;
  Individual *other = (Individual *) b;
  int m = *(int*)idx;

  if (self->objs[m] > other->objs[m]) return 1;
  else if (self->objs[m] < other->objs[m]) return -1;
  else return 0;
}

/* calculate crowding distance for one front */
void calculate_crowd_distance(NSGAIIVals *nsga2, Pool *p, int rank) {
  assert(p->fronts != NULL && p->fronts_sz > 0);
  Population front_beg, front_end;
  size_t frontsz;
  if (p->nrank > 0) {
    get_rank_tuple(rank, p, front_end, front_beg);
    get_frontsz(front_end, front_beg);
    for (Population ptr = front_beg; ptr <= front_end; ptr++) {
      ptr->crowd_dist = 0;
    }
    for (int m = 0; m < nsga2->nobjs; m++) {
      /* sort by objective, gcc only */
      qsort_r(front_beg, frontsz, sizeof(Individual), cmp_objective, &m);
      front_beg->crowd_dist = nsga2->maxobj;
      front_end->crowd_dist = nsga2->minobj;
      for (int i = 1; i < frontsz - 1; i++) {
        front_beg[i].crowd_dist =
            (front_beg[i + 1].crowd_dist - front_beg[i - 1].crowd_dist) /
            (nsga2->maxobj - nsga2->minobj);
      }
    }
  }
}

bool crowding_operator(Individual *self, Individual *other) {
  return self->rank < other->rank ||
         (self->rank == other->rank && self->crowd_dist > other->crowd_dist);
}
