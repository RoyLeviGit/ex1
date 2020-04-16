#include "election.h"
#include "area.h"
#include "tribe.h"
#include "vote.h"

#define NO_INDEX -1

typedef struct election
{
    Map areas;
    Map tribes;
    Map votes;
} *Election;

static ElectionResult outOfMemory() {
    
}

static ElectionResult isAreaExists(Election election, int area_id) {
    assert(election != NULL);
    char* iterator= malloc(sizeof(*iterator)*MAX_AREA_KEY_LENGTH);
     if (iterator == NULL){
        return ELECTION_OUT_OF_MEMORY;
    }
    MAP_FOREACH(iterator, election->areas) {
        char* value = MapGet(election->areas, iterator);
        Area area = stringToArea(iterator, value);
        if (area == NULL) {
            ELECTION_NULL_ARGUMENT;
        }
        if (area->id == area_id) {
            free(iterator);
            areaDestroy(area); 
            return ELECTION_AREA_ALREADY_EXIST;
        }
        areaDestroy(area);
    }
    free(iterator);
    return ELECTION_AREA_NOT_EXIST;
}

static ElectionResult isTribeExists(Election election, int tribe_id) {
    assert(election != NULL);
    char* iterator = malloc(sizeof(*iterator)*MAX_AREA_KEY_LENGTH);
    if (iterator == NULL){
        return ELECTION_OUT_OF_MEMORY;
    }
    MAP_FOREACH(iterator, election->tribes) {
        char* value = MapGet(election->tribes, iterator);
        Tribe tribe = stringToTribe(iterator, value);
        if (tribe == NULL) {
            return ELECTION_NULL_ARGUMENT;
        }
        if (tribe->id == tribe_id) {
            free(iterator);
            tribeDestroy(tribe); 
            return ELECTION_TRIBE_ALREADY_EXIST;
        }
        tribeDestroy(tribe);
    }
    free(iterator);
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
    ElectionResult check = isAreaExists(election,area_id);
    if(check == ELECTION_NULL_ARGUMENT){
        return ELECTION_NULL_ARGUMENT;
    }
    if (check == ELECTION_TRIBE_ALREADY_EXIST) {
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if (!isValidTribeName(area_name)) {
        return ELECTION_INVALID_NAME;
    }
    Area area = createTribe(area_id, area_name);
    if (area == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    //logic
    char* key = NULL , value = NULL;
    if (AreaToString(area, key, value) == OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    assert(key != NULL, value != NULL);
    mapPut(election->areas, key, value);
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
    ElectionResult check = isTribeExists(election,tribe_id);
    if (check == NULL){
        return ELECTION_NULL_ARGUMENT;
    }
    if (check == ELECTION_TRIBE_ALREADY_EXIST) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (!isValidTribeName(tribe_name)) {
        return ELECTION_INVALID_NAME;
    }
    Tribe tribe = createTribe(tribe_id, tribe_name);
    if (tribe==NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    // logic
    char* key = NULL, value = NULL;
    if (tribeToString(tribe, key, value) == OUT_OF_MEMORY) {
        return outOfMemory();
    }
    assert(key != NULL, value != NULL);
    mapPut(election->tribes, key, value);
    free(key);
    free(value);
    return ELECTION_SUCCESS;
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
        return outOfMemory();
    }

    // logic
    char* key = NULL, value = NULL;
    if (voteValueToString(vote, key, value) == OUT_OF_MEMORY) {
        return outOfMemory();
    }
    assert(key != NULL, value != NULL);
    mapPut(election->votes, key, value);
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
        return outOfMemory();
    }

    // logic
    char* key = NULL, value = NULL;
    if (voteValueToString(new_vote, key, value) == OUT_OF_MEMORY) {
        return outOfMemory();
    }
    assert(key != NULL, value != NULL);
    // get old and new vote
    char* old_value = mapGet(election->votes, key);
    Vote old_vote = stringToVote(key, value);
    if (old_vote == NULL) {
        free(key);
        free(value);
        return outOfMemory();
    }
    // change votes
    new_vote->votes = old_vote->votes - new_vote->votes;
    if (isValidNumberOfVotes(new_vote->votes)) {
        mapRemove(election->votes, key);
    } else {
        // get new_vote new value and change it in the map
        if (voteValueToString(new_vote, key, value) == OUT_OF_MEMORY) {
            return outOfMemory();
        }
        assert(key != NULL, value != NULL);
        mapPut(election->votes, key, value);
    }
    free(key);
    free(value);
    return ELECTION_SUCCESS;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name){
        if(isTribeExists(election, tribe_id)) {

        }
        return electionAddTribe(election, tribe_id, tribe_name);
}
