#ifndef VOTE_H
#define VOTE_H

#include "utilities.h"

//Decleration of the struct vote*
typedef struct vote *Vote;

/**
* voteCreate: Allocates a new vote.
* @param area_id - the id of the area, must be greater or equal to zero
* @param tribe_id - the id of the tribe, must be greater or equal to zero
* @param votes - the number of votes of the given area to the given tribe
* @return
* 	NULL - if allocations failed.
* 	A new vote in case of success.
*/
Vote voteCreate(int area_id, int tribe_id, int votes);

/**
* voteDestroy: Deallocates an existing vote.
*
* @param vote - Target area to be deallocated. If area is NULL nothing will be
* 		done
*/
void voteDestroy(Vote vote);

/**
* getVoteAreaID: returns the id of the area of the vote
*
* @param vote - Target vote.
* @return
*   id of the area 
*/
int getVoteAreaID(Vote vote);

/**
* getVoteTribeID: returns the id of the tribe of the vote
*
* @param vote - Target vote.
* @return
*   id of the tribe 
*/
int getVoteTribeID(Vote vote);

/**
* getVoteNumberOfVotes: returns the number of votes of the vote
*
* @param vote - Target vote.
* @return
*    the number of votes of the vote
*/
int getVoteNumberOfVotes(Vote vote);

/**
* setVoteNumberOfVotes: puts the given number of votes in a specific vote
*
* @param vote - Target vote.
* @param votes - number of votes to put in vote. puts 0 if invalid votes were given.
*/
void setVoteNumberOfVotes(Vote vote, int votes);

/**
* isValidNumberOfVotes: returns if the given number of votes is valid
*
* @param num_of_votes - int num_of_votes.
* @return
*   true - if the given num_of_votes is valid, false - num_of_votes is not valid.
*/
bool isValidNumberOfVotes(int num_of_votes);

/**
* stringToVote: gets key and value and creates a new vote
*
* @param key - char* key.
* @param value - char* value. 
* @return
*   a new vote made out of the key that became the id's and the value that is the number of votes
*/
Vote stringToVote(char* key, char* value);

/**
* voteToString: gets key and value and a vote, and puts area id and tribe id in key 
*               and number of vote in value
*
* @param vote - Vote vote 
* @param key - char** key.
* @param value - char** value.
* @return
*   OUT_OF_MEMORY - if allocations of key or value failed. SUCCESS - if the values where properly put
*/
Result voteToString(Vote vote, char** key, char** value);

#endif //VOTE_H
