#include "vote.h"
#include <stdlib.h>

#define EMPTY 0

struct vote {
    int area_id;
    int tribe_id;
    int votes;
};

Vote voteCreate(int area_id, int tribe_id, int votes) {
    Vote vote = malloc(sizeof(*vote));
    if (vote == NULL) {
        return NULL;
    }
    vote->area_id = area_id;
    vote->tribe_id = tribe_id;
    vote->votes = votes;
    return vote;
}

void voteDestroy(Vote vote) {
    free(vote);
}
int getVoteAreaID(Vote vote){
    assert(vote != NULL);
    return vote->area_id;
}

int getVoteTribeID(Vote vote){
    assert(vote != NULL);
    return vote->tribe_id;
}

int getVoteNumberOfVotes(Vote vote){
    assert(vote != NULL);
    return vote->votes;
}

void setVoteNumberOfVotes(Vote vote, int votes){
    assert(vote != NULL);
    if(!isValidNumberOfVotes(votes)){
        vote->votes = 0;
    } else {
        vote->votes = votes;  
    }
}

bool isValidVoteID(int id) {
    return id >= 0;
}

bool isValidNumberOfVotes(int num_of_votes){
    if (num_of_votes <= EMPTY) {
        return false;
    }
    return true;
}

Vote stringToVote(char* key, char* value) {
    assert(key != NULL);
    assert(value != NULL);
    int area_id, tribe_id, votes;
    if (stringToTwoNumbers(key, &area_id, &tribe_id) == OUT_OF_MEMORY) {
        return NULL;
    }
    votes = stringToInt(value);
    assert(votes != CONVERT_ERROR);
    return voteCreate(area_id, tribe_id, votes);
}

Result voteToString(Vote vote, char** key, char** value) {
    assert(vote != NULL);
    if(twoNumbersToString(vote->area_id, vote->tribe_id, key) == OUT_OF_MEMORY){
        return OUT_OF_MEMORY;
    }
    assert(*key != NULL);
    *value = intToString(vote->votes);
    if(*value == NULL) {
        return OUT_OF_MEMORY;
    } 
    return SUCCESS;
}
