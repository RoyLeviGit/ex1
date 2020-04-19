#include "election.h"
#include "area.h"
#include "tribe.h"
#include "vote.h"

#define NO_INDEX -1
#define GARBAGE_VALUE ""

//TODO: voteDestroy, and double check every function return for correct free() usage.

typedef struct election
{
    Map areas;
    Map tribes;
    Map votes;
} *Election;

static ElectionResult isAreaExists(Election election, int area_id) {
    assert(election != NULL);
    MAP_FOREACH(key, election->areas) {
        Area area = stringToArea(key, GARBAGE_VALUE);
        if (area == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if (area->id == area_id) {
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
        if (tribe->id == tribe_id) {
            return ELECTION_TRIBE_ALREADY_EXIST;
        }
        tribeDestroy(tribe);
    }
    return ELECTION_TRIBE_NOT_EXIST;
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name){
    //error cases
    if (election == NULL || area_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidTribeID(area_id)) {
        return ELECTION_INVALID_ID;
    }
    ElectionResult search_result = isAreaExists(election, area_id);
    if (search_result == ELECTION_AREA_ALREADY_EXIST) {
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if (!isValidAreaName(area_name)) {
        return ELECTION_INVALID_NAME;
    }
    Area area = createTribe(area_id, area_name);
    if (area == NULL || search_result == ELECTION_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }

    //logic
    char* key = NULL , value = NULL;
    if (AreaToString(area, key, value) == OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL, value != NULL);
    mapPut(election->areas, key, value);
    return ELECTION_SUCCESS;
}
static ElectionResult tribeInsert(Election election, Tribe tribe){
    char* key = NULL, value = NULL;
    if (tribeToString(tribe, key, value) == OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL, value != NULL);
    mapPut(election->tribes, key, value);
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
    Tribe tribe = createTribe(tribe_id, tribe_name);
    if (tribe == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    // logic
    return tribeInsert(election, tribe);
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
        return ELECTION_OUT_OF_MEMORY;
    }

    // logic
    char* key = NULL, value = NULL;
    if (voteValueToString(vote, key, value) == OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL, value != NULL);
    mapPut(election->votes, key, value);
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
        return ELECTION_OUT_OF_MEMORY;
    }

    // logic
    char* key = NULL, value = NULL;
    if (voteValueToString(new_vote, key, value) == OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL, value != NULL);
    // get old and new vote
    char* old_value = mapGet(election->votes, key);
    Vote old_vote = stringToVote(key, value);
    if (old_vote == NULL) {
        free(key);
        free(value);
        return ELECTION_OUT_OF_MEMORY;
    }
    // change votes
    new_vote->votes = old_vote->votes - new_vote->votes;
    if (isValidNumberOfVotes(new_vote->votes)) {
        mapRemove(election->votes, key);
    } else {
        // get new_vote new value and change it in the map
        if (voteValueToString(new_vote, key, value) == OUT_OF_MEMORY) {
            free(key);
            free(value);
            return ELECTION_OUT_OF_MEMORY;
        }
        assert(key != NULL, value != NULL);
        MapResult map_result = mapPut(election->votes, key, value);
        if (map_result == MAP_OUT_OF_MEMORY) {
            free(key);
            free(value);
            return ELECTION_OUT_OF_MEMORY;
        }
    }
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
    ElectionResult check = isTribeExists(election,tribe_id);
    if (check == NULL){
        return ELECTION_NULL_ARGUMENT;
    }
    if (check == ELECTION_TRIBE_NOT_EXIST) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (!isValidTribeName(tribe_name)) {
        return ELECTION_INVALID_NAME;
    }
    Tribe tribe = createTribe(tribe_id, tribe_name);
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
        return ELECTION_OUT_OF_MEMORY;
    }
    char* key = NULL, value = NULL;
    if (tribeToString(tribe, key, value) == OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL, value != NULL);
    mapRemove(election->tribes, key);

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
        if (should_delete_area(area->id)) {
            mapRemove(election->areas, key);
        }
        areaDestroy(area);
    }
    return ELECTION_SUCCESS;
}

// Gets tribe_id and num_of_votes from value string
static ElectionResult stringToTribeMapping(char* value, int* tribe_id, int* num_of_votes) {

}

// Return value string from tribe_id and num_of_votes 
static ElectionResult tribeMappingToString(int tribe_id, int num_of_votes, char** value) {

}

Map electionComputeAreasToTribesMapping (Election election) {
    if (election == NULL) {
        return NULL;
    }
    Map celebTribes = mapCreate();

    MAP_FOREACH(vote_key, election->votes) {
        char* vote_value = mapGet(election->votes, vote_key);
        Vote vote = stringToVote(vote_key, vote_value);
        if (vote == NULL) {
            return NULL;
        }
        
        char* area_key = intToString(vote->area_id);
        char* area_current_value = mapGet(celebTribes, area_key);
        if (area_current_value == NULL) {
            ElectionResult result = tribeMappingToString(vote->tribe_id, vote->votes, &area_current_value);
            if (result == ELECTION_OUT_OF_MEMORY) {
                return NULL;
            }
            assert(area_current_value != NULL);
            MapResult map_result = mapPut(celebTribes, area_key, area_current_value);
            if (map_result == MAP_OUT_OF_MEMORY) {
                return NULL;
            }
            continue;
        }
        int current_tribe_id, current_num_of_votes;
        ElectionResult result = stringToTribeMapping(area_current_value, &current_tribe_id, &current_num_of_votes);
        if (result == ELECTION_OUT_OF_MEMORY) {
            return NULL;
        }
        if (vote->votes > current_num_of_votes 
        || (vote->votes == current_num_of_votes && vote->tribe_id < current_tribe_id)) {
            ElectionResult result = tribeMappingToString(vote->tribe_id, vote->votes, &area_current_value);
            if (result == ELECTION_OUT_OF_MEMORY) {
                return NULL;
            }
            assert(area_current_value != NULL);
            MapResult map_result = mapPut(celebTribes, area_key, area_current_value);
            if (map_result == MAP_OUT_OF_MEMORY) {
                return NULL;
            }
        }
    }

    MAP_FOREACH(key, celebTribes) {
        char* value = mapGet(celebTribes, key);
        int desired_tribe_id, not_so_desired_votes;
        ElectionResult result = stringToTribeMapping(value, &desired_tribe_id, &not_so_desired_votes);
        if (result == ELECTION_OUT_OF_MEMORY) {
            return NULL;
        }
        char* desired_tribe_id_string = intToString(desired_tribe_id);
        MapResult map_result = mapPut(celebTribes, key, desired_tribe_id_string);
        if (map_result == MAP_OUT_OF_MEMORY) {
            return NULL;
        }
    }
}
