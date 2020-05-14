#include "crowddist.h"
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

/* calculate crowding distance for one front */
void get_crowd_distance(NSGAIIVals *nsga2, Pool *p, int rank) {
  Population *front_beg, *front_end;
  int frontsz;
  if (p->nrank > 0) {
    get_rank_tuple(rank, p, front_end, front_beg);
    get_frontsz(front_end, front_beg);
    for (Population *ptr = front_beg; ptr <= front_end; ptr++) {
      (*ptr)->crowd_dist = 0;
    }
    for (int m = 0; m < nsga2->nobjs; m++) {
      // qsort(front_beg, frontsz, sizeof(*front_beg), );
      (*front_beg)->crowd_dist = nsga2->maxobj;
      for (int i = 1; i < frontsz - 1; i++) {
        front_beg[i]->crowd_dist =
            (front_beg[i + 1]->crowd_dist - front_beg[i - 1]->crowd_dist) /
            (nsga2->maxobj - nsga2->minobj);
      }
    }
  }
}

bool crowding_operator(NSGAIIVals *nsga2, Individual *self, Individual *other) {
  return self->rank < other->rank ||
         (self->rank == other->rank && self->crowd_dist > other->crowd_dist);
}
