#ifndef VOTE_H
#define VOTE_H

#include "utilities.h"
#include "area.h"
#include "tribe.h"

#define MAX_VOTE_KEY_LENGTH 20
#define MAX_VOTE_VALUE_LENGTH 20

typedef struct vote {
    char* area_id;
    char* tribe_id;
    int votes;
} *Vote;

Vote voteCreate(char* area_id, char* tribe_id, unsigned int votes);

bool isValidNumberOfVotes(int num_of_votes);

Vote stringToVote(char* key, char* value);

Result voteToString(Vote vote, char* key, char* value);

#endif //VOTE_H
