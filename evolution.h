#ifndef _EVOLUTION
#define _EVOLUTION
#include "defs.h"
#include "utils.h"

/* return the poplulation front.
 * front size will be returned from *front_sz */
Population evolve(NSGA2ctx *nsga2, Problem *problem, size_t *front_sz);

#endif /* ifndef _EVOLUTION */
