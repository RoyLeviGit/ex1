#ifndef VOTE_H
#define VOTE_H

#include "area.h"
#include "tribe.h"

typedef struct vote {
    Area area;
    Tribe tribe;
    unsigned int votes;
} *Vote;

#endif //VOTE_H
