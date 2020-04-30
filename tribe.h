#ifndef TRIBE_H_
#define TRIBE_H_

#include "utilities.h"

//Decleration of the struct tribe*
typedef struct tribe *Tribe;

/**
* tribeCreate: Allocates a new tribe.
* @param id - the id of the tribe, must be greater or equal to zero
* @param name - the name of the tribe
* @return
* 	NULL - if allocations failed, NULL was sent or invalid id was sent.
* 	A new Tribe in case of success.
*/
Tribe tribeCreate(int id, const char* name);

/**
* tribeDestroy: Deallocates an existing tribe.
*
* @param tribe - Target tribe to be deallocated. If tribe is NULL nothing will be
* 		done
*/
void tribeDestroy(Tribe tribe);

/**
* getTribeID: returns the id of a specific tribe
*
* @param tribe - Target tribe.
* @return
*   id of the tribe 
*/
int getTribeID(Tribe tribe);

/**
* getTribeName: returns the name of target tribe
*
* @param tribe - Target tribe.
* @return
*   pointer to the name of the tribe.
*/
char* getTribeName(Tribe tribe);

/**
* isValidTribeID: returns if the given id is valid
*
* @param id - int id.
* @return
*   true - if the given id is valid, false - id is not valid.
*/
bool isValidTribeID(int id);

/**
* isValidTribeName: returns if the given tribe name is valid
*
* @param name - char* name.
* @return
*   true - if the given name is valid, false - name is not valid.
*/
bool isValidTribeName(const char* name);

/**
* stringToTribe: gets key and value and creates a new tribe
*
* @param key - char* key.
* @param value - char* value. 
* @return
*   a new tribe made out of the key that became the id and the value that is the name
*/
Tribe stringToTribe(char* key, char* value);

/**
* tribeToString: gets key and value and a tribe, and puts tribe id in key and tribe name in value
*
* @param tribe - Tribe tribe 
* @param key - char** key.
* @param value - char** value.
* @return
*   OUT_OF_MEMORY - if allocations of key or value failed. SUCCESS - if the values where properly put
*/
Result tribeToString(Tribe tribe, char** key, char** value);

#endif //TRIBE_H_
