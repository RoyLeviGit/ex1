#include "election.h"
#include "vote.h"

#define NO_INDEX -1

typedef struct election
{
    Area *areas;
    int areas_size;
    int areas_max_size;

    Tribe *tribes;
    int tribes_size;
    int tribes_max_size;

    Vote *votes;
    int votes_size;
    int votes_max_size;
} *Election;

static int indexOfVote(Election election, int area_id, int tribe_id) {
    for (int i = 0;  i < election->votes_size; i++) {
        if (election->votes[i]->area->id == area_id && election->votes[i]->tribe->id == tribe_id) {
            return i;
        }
    }
    return NO_INDEX;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes) {
    
}