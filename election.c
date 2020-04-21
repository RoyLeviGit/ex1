#include "election.h"
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"
#include "area.h"
#include "tribe.h"
#include "vote.h"

#define NO_INDEX -1
#define GARBAGE_VALUE ""

struct election_t {
    Map areas;
    Map tribes;
    Map votes;
};

Election electionCreate() {
    Election election = malloc(sizeof(*election));
    if (election == NULL) {
        return NULL;
    }
    election->areas = mapCreate();
    election->tribes = mapCreate();
    election->votes = mapCreate();
    if (election->areas == NULL || election->tribes == NULL || election->votes == NULL) {
        electionDestroy(election);
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) {
    if (election == NULL) {
        return;
    }
    mapDestroy(election->areas);
    mapDestroy(election->tribes);
    mapDestroy(election->votes);
    free(election);
    return;
}

static ElectionResult isAreaExists(Election election, int area_id) {
    assert(election != NULL);
    MAP_FOREACH(key, election->areas) {
        Area area = stringToArea(key, GARBAGE_VALUE);
        if (area == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if (getAreaID(area) == area_id) {
            areaDestroy(area); 
            return ELECTION_AREA_ALREADY_EXIST;
        }
        areaDestroy(area);
    }
    return ELECTION_AREA_NOT_EXIST;
}

static ElectionResult isTribeExists(Election election, int tribe_id) {
    assert(election != NULL);
    MAP_FOREACH(key, election->tribes) {
        Tribe tribe = stringToTribe(key, GARBAGE_VALUE);
        if (tribe == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if (getTribeID(tribe) == tribe_id) {
            tribeDestroy(tribe);
            return ELECTION_TRIBE_ALREADY_EXIST;
        }
        tribeDestroy(tribe);
    }
    return ELECTION_TRIBE_NOT_EXIST;
}

// inserts tribe to election map and destroys it after usage
static ElectionResult tribeInsert(Election election, Tribe tribe) {
    char *key = NULL, *value = NULL;
    if (tribeToString(tribe, &key, &value) == OUT_OF_MEMORY) {
        tribeDestroy(tribe);
        free(key);
        free(value);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL);
    assert(value != NULL);
    mapPut(election->tribes, key, value);

    tribeDestroy(tribe);
    free(key);
    free(value);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) {
    // error cases
    if (election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidTribeID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    ElectionResult tribe_search_result = isTribeExists(election,tribe_id);
    if (tribe_search_result == ELECTION_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if (tribe_search_result == ELECTION_TRIBE_ALREADY_EXIST) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (!isValidTribeName(tribe_name)) {
        return ELECTION_INVALID_NAME;
    }
    Tribe tribe = tribeCreate(tribe_id, tribe_name);
    if (tribe == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    // logic
    return tribeInsert(election, tribe);
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name){
    //error cases
    if (election == NULL || area_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidAreaID(area_id)) {
        return ELECTION_INVALID_ID;
    }
    ElectionResult search_result = isAreaExists(election, area_id);
    if (search_result == ELECTION_AREA_ALREADY_EXIST) {
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if (!isValidAreaName(area_name)) {
        return ELECTION_INVALID_NAME;
    }
    Area area = areaCreate(area_id, area_name);
    if (area == NULL || search_result == ELECTION_OUT_OF_MEMORY) {
        areaDestroy(area);
        return ELECTION_OUT_OF_MEMORY;
    }

    //logic
    char *key = NULL , *value = NULL;
    if (areaToString(area, &key, &value) == OUT_OF_MEMORY) {
        areaDestroy(area);
        free(key);
        free(value);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL);
    assert(value != NULL);
    mapPut(election->areas, key, value);
    
    areaDestroy(area);
    free(key);
    free(value);
    return ELECTION_SUCCESS;
}

char* electionGetTribeName (Election election, int tribe_id) {
    if (election == NULL) {
        return NULL;
    }
    if (!isValidTribeID(tribe_id)) {
        return NULL;
    }
    
    MAP_FOREACH(key, election->tribes) {
        Tribe tribe = stringToTribe(key, GARBAGE_VALUE);
        if (tribe == NULL) {
            tribeDestroy(tribe);
            return NULL;
        }
        if (getTribeID(tribe) == tribe_id) {
            tribeDestroy(tribe);
            // return copy of tribe name
            char* value = mapGet(election->tribes,key);
            tribe = stringToTribe(key, value);
            char* name = malloc(sizeof(*name)*(strlen(getTribeName(tribe)+1)));
            if (name == NULL) {
                tribeDestroy(tribe);
                return NULL;
            }
            strcpy(name, getTribeName(tribe));
            tribeDestroy(tribe);
            return name;
        }
        tribeDestroy(tribe);
    }
    return NULL;
}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    // error cases
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidAreaID(area_id) || !isValidTribeID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    if (!isValidNumberOfVotes(num_of_votes)) {
        return ELECTION_INVALID_VOTES;
    }
    ElectionResult area_search_result = isAreaExists(election, area_id);
    if (area_search_result == ELECTION_AREA_NOT_EXIST) {
        return ELECTION_AREA_NOT_EXIST;
    } 
    ElectionResult tribe_search_result = isTribeExists(election, tribe_id);
    if (tribe_search_result == ELECTION_TRIBE_NOT_EXIST) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    Vote vote = voteCreate(area_id, tribe_id, num_of_votes);
    if (vote == NULL || area_search_result == ELECTION_OUT_OF_MEMORY 
    || tribe_search_result == ELECTION_OUT_OF_MEMORY) {
        voteDestroy(vote);
        return ELECTION_OUT_OF_MEMORY;
    }

    // logic
    char *key = NULL, *value = NULL;
    if (voteToString(vote, &key, &value) == OUT_OF_MEMORY) {
        voteDestroy(vote);
        free(key);
        free(value);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL);
    assert(value != NULL);
    mapPut(election->votes, key, value);

    voteDestroy(vote);
    free(key);
    free(value);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes) {
    // error cases
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidAreaID(area_id) || !isValidTribeID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    if (!isValidNumberOfVotes(num_of_votes)) {
        return ELECTION_INVALID_VOTES;
    }
    ElectionResult area_search_result = isAreaExists(election, area_id);
    if (area_search_result == ELECTION_AREA_NOT_EXIST) {
        return ELECTION_AREA_NOT_EXIST;
    } 
    ElectionResult tribe_search_result = isTribeExists(election, tribe_id);
    if (tribe_search_result == ELECTION_TRIBE_NOT_EXIST) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    // new_vote holds number of votes to change
    Vote new_vote = voteCreate(area_id, tribe_id, num_of_votes);
    if (new_vote == NULL || area_search_result == ELECTION_OUT_OF_MEMORY 
    || tribe_search_result == ELECTION_OUT_OF_MEMORY) {
        voteDestroy(new_vote);
        return ELECTION_OUT_OF_MEMORY;
    }

    // logic
    char *key = NULL, *value = NULL;
    if (voteToString(new_vote, &key, &value) == OUT_OF_MEMORY) {
        voteDestroy(new_vote);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL);
    assert(value != NULL);
    // get old and new vote
    char* old_value = mapGet(election->votes, key);
    if (old_value == NULL) {
        // no previous value, so do nothing.
        voteDestroy(new_vote);
        free(key);
        free(value);
        return ELECTION_SUCCESS;
    }

    Vote old_vote = stringToVote(key, old_value);
    if (old_vote == NULL) {
        voteDestroy(new_vote);
        free(key);
        free(value);
        return ELECTION_OUT_OF_MEMORY;
    }
    // change votes
    setVoteNumberOfVotes(new_vote, getVoteNumberOfVotes(old_vote) - getVoteNumberOfVotes(new_vote));
    if (getVoteNumberOfVotes(new_vote) == 0) {
        mapRemove(election->votes, key);
    } else {
        // get new_vote new value and change it in the map
        if (voteToString(new_vote, &key, &value) == OUT_OF_MEMORY) {
            voteDestroy(new_vote);
            voteDestroy(old_vote);
            free(key);
            free(value);
            return ELECTION_OUT_OF_MEMORY;
        }
        assert(key != NULL);
        assert(value != NULL);
        MapResult result = mapPut(election->votes, key, value);
        if (result == MAP_OUT_OF_MEMORY) {
            voteDestroy(new_vote);
            voteDestroy(old_vote);
            free(key);
            free(value);
            return ELECTION_OUT_OF_MEMORY;
        }
    }
    voteDestroy(new_vote);
    voteDestroy(old_vote);
    free(key);
    free(value);
    return ELECTION_SUCCESS;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name){
    if (election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidTribeID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    if (isTribeExists(election,tribe_id) == ELECTION_TRIBE_NOT_EXIST) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (!isValidTribeName(tribe_name)) {
        return ELECTION_INVALID_NAME;
    }
    Tribe tribe = tribeCreate(tribe_id, tribe_name);
    if (tribe == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    return tribeInsert(election, tribe);
}

ElectionResult electionRemoveTribe (Election election, int tribe_id) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidTribeID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    ElectionResult search_result = isTribeExists(election, tribe_id);
    if (search_result == ELECTION_TRIBE_NOT_EXIST) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    Tribe tribe = tribeCreate(tribe_id, GARBAGE_VALUE);
    if (tribe == NULL || search_result == ELECTION_OUT_OF_MEMORY) {
        tribeDestroy(tribe);
        return ELECTION_OUT_OF_MEMORY;
    }

    // remove tribe from tribes map
    char *tribe_key = NULL, *tribe_value = NULL;
    if (tribeToString(tribe, &tribe_key, &tribe_value) == OUT_OF_MEMORY) {
        tribeDestroy(tribe);
        free(tribe_key);
        free(tribe_value);
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(tribe_key != NULL);
    assert(tribe_value != NULL);
    mapRemove(election->tribes, tribe_key);

    tribeDestroy(tribe);
    free(tribe_key);
    free(tribe_value);

    // remove tribe from votes map
    MAP_FOREACH(vote_key, election->votes) {
        Vote vote = stringToVote(vote_key, GARBAGE_VALUE);
        if(vote == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if (getVoteTribeID(vote) == tribe_id) {
            mapRemove(election->votes, vote_key);
        }
        voteDestroy(vote);
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {
    if (election == NULL || should_delete_area == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    MAP_FOREACH(key, election->areas) {
        Area area = stringToArea(key, GARBAGE_VALUE);
        if (area == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if (should_delete_area(getAreaID(area))) {
            mapRemove(election->areas, key);
        }
        areaDestroy(area);
    }
    return ELECTION_SUCCESS;
}

Map electionComputeAreasToTribesMapping (Election election) {
    if (election == NULL) {
        return NULL;
    }
    Map celebTribes = mapCreate();

    // find smallest tribe id
    char* first_tribe_key = mapGetFirst(election->tribes);
    if (first_tribe_key == NULL) {
        // no tribe means there are no votes
        return celebTribes;
    }
    Tribe first_tribe = stringToTribe(first_tribe_key, GARBAGE_VALUE);
    int smallest_tribe_id = getTribeID(first_tribe); 
    tribeDestroy(first_tribe);

    MAP_FOREACH(tribe_key, election->tribes) {
        Tribe tribe = stringToTribe(tribe_key, GARBAGE_VALUE);
        if (tribe == NULL) {
            mapDestroy(celebTribes);
            return NULL;
        }
        if (getTribeID(tribe) < smallest_tribe_id) {
            smallest_tribe_id = getTribeID(tribe);
        }
        tribeDestroy(tribe);
    }

    // put smallest tribe id 
    MAP_FOREACH(area_key, election->areas) {
        char* initial_value = NULL;
        Result result = twoNumbersToString(smallest_tribe_id, 0, &initial_value);
        if (result == OUT_OF_MEMORY) {
            mapDestroy(celebTribes);
            return NULL;
        }
        assert (initial_value != NULL);
        MapResult map_result = mapPut(celebTribes, area_key, initial_value);
        if (map_result == MAP_OUT_OF_MEMORY) {
            mapDestroy(celebTribes);
            free(initial_value);
            return NULL;
        }
    }

    MAP_FOREACH(vote_key, election->votes) {
        char* vote_value = mapGet(election->votes, vote_key);
        assert(vote_value != NULL);
        Vote vote = stringToVote(vote_key, vote_value);
        if (vote == NULL) {
            mapDestroy(celebTribes);
            return NULL;
        }
        char* area_key = intToString(getVoteAreaID(vote));
        if (area_key == NULL) {
            mapDestroy(celebTribes);
            voteDestroy(vote);
            return NULL;
        }
        char* area_value = mapGet(celebTribes, area_key);
        
        if (area_value == NULL) {
            // area not currently in celebs map
            Result result = twoNumbersToString(getVoteTribeID(vote), getVoteNumberOfVotes(vote), &area_value);
            if (result == OUT_OF_MEMORY) {
                mapDestroy(celebTribes);
                voteDestroy(vote);
                free(area_key);
                return NULL;
            }
            assert(area_value != NULL);
            MapResult map_result = mapPut(celebTribes, area_key, area_value);
            if (map_result == MAP_OUT_OF_MEMORY) {
                mapDestroy(celebTribes);
                voteDestroy(vote);
                free(area_key);
                free(area_value);
                return NULL;
            }
        } else {
            // area already in celebs map
            int current_tribe_id, current_num_of_votes;
            if (stringToTwoNumbers(area_value, &current_tribe_id, &current_num_of_votes) == OUT_OF_MEMORY) {
                mapDestroy(celebTribes);
                voteDestroy(vote);
                free(area_key);
                return NULL;
            }

            if (getVoteNumberOfVotes(vote) > current_num_of_votes 
            || (getVoteNumberOfVotes(vote) == current_num_of_votes && getVoteTribeID(vote) < current_tribe_id)) {
                Result result = twoNumbersToString(getVoteTribeID(vote), getVoteNumberOfVotes(vote), &area_value);
                if (result == OUT_OF_MEMORY) {
                    mapDestroy(celebTribes);
                    voteDestroy(vote);
                    free(area_key);
                    return NULL;
                }
                assert(area_value != NULL);
                MapResult map_result = mapPut(celebTribes, area_key, area_value);
                if (map_result == MAP_OUT_OF_MEMORY) {
                    mapDestroy(celebTribes);
                    voteDestroy(vote);
                    free(area_key);
                    free(area_value);
                    return NULL;
                }
            }
        }
        voteDestroy(vote);
        free(area_key);  
    }

    // set correct value format
    MAP_FOREACH(key, celebTribes) {
        char* value = mapGet(celebTribes, key);
        int desired_tribe_id, not_so_desired_votes;
        if (stringToTwoNumbers(value, &desired_tribe_id, &not_so_desired_votes) == OUT_OF_MEMORY) {
            mapDestroy(celebTribes);
            return NULL;
        }

        char* desired_tribe_id_string = intToString(desired_tribe_id);
        if (desired_tribe_id_string == NULL) {
            mapDestroy(celebTribes);
            return NULL;
        }
        MapResult map_result = mapPut(celebTribes, key, desired_tribe_id_string);
        if (map_result == MAP_OUT_OF_MEMORY) {
            mapDestroy(celebTribes);
            free(desired_tribe_id_string);
            return NULL;
        }
        free(desired_tribe_id_string);
    }
    
    return celebTribes;
}
