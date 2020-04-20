#ifndef VOTE_H
#define VOTE_H

#include "utilities.h"

typedef struct vote {
    int area_id;
    int tribe_id;
    int votes;
} *Vote;

Vote voteCreate(int area_id, int tribe_id, int votes);

void voteDestroy(Vote vote);

bool isValidNumberOfVotes(int num_of_votes);

Vote stringToVote(char* key, char* value);

Result voteToString(Vote vote, char** key, char** value);

#endif //VOTE_H
