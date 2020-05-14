#include "crowddist.h"
#include <stdbool.h>

void get_crowd_distance(NSGAIIVals *nsga2, Pool *p, size_t front_sz) {

}

bool crowding_operator(NSGAIIVals *nsga2, Individual *self, Individual *other) {
  return self->rank < other->rank ||
         (self->rank == other->rank && self->crowd_dist > other->crowd_dist);
}
