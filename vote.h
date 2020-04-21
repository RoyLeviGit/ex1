#ifndef VOTE_H
#define VOTE_H

#include "utilities.h"

typedef struct vote *Vote;

Vote voteCreate(int area_id, int tribe_id, int votes);

void voteDestroy(Vote vote);

int getVoteAreaID(Vote vote);

int getVoteTribeID(Vote vote);

int getVoteNumberOfVotes(Vote vote);

void setVoteNumberOfVotes(Vote vote, int votes);

bool isValidNumberOfVotes(int num_of_votes);

Vote stringToVote(char* key, char* value);

Result voteToString(Vote vote, char** key, char** value);

#endif //VOTE_H
