#ifndef AREA_H
#define AREA_H

#include "utilities.h"

//Decleration of the struct area*
typedef struct area *Area;

/**
* areaCreate: Allocates a new area.
* @param id - the id of the area, must be greater or equal to zero
* @param name - the name of the area
* @return
* 	NULL - if allocations failed, NULL was sent or invalid id was sent.
* 	A new area in case of success.
*/
Area areaCreate(int id, const char* name);

/**
* areaDestroy: Deallocates an existing area.
*
* @param area - Target area to be deallocated. If area is NULL nothing will be
* 		done
*/
void areaDestroy(Area area);

/**
* getAreaID: returns the id of a specific area
*
* @param area - Target area.
* @return
*   id of the area 
*/
int getAreaID(Area area);

/**
* getAreaName: returns the name of target area
*
* @param area - Target area.
* @return
*   pointer to the name of the area.
*/
char* getAreaName(Area area);

/**
* isValidAreaID: returns if the given id is valid
*
* @param id - int id.
* @return
*   true - if the given id is valid, false - id is not valid.
*/
bool isValidAreaID(int id);

/**
* isValidAreaName: returns if the given area name is valid
*
* @param name - char* name.
* @return
*   true - if the given name is valid, false - name is not valid.
*/
bool isValidAreaName(const char* name);

/**
* stringToArea: gets key and value and creates a new area
*
* @param key - char* key.
* @param value - char* value. 
* @return
*   a new area made out of the key that became the id and the value that is the name
*/
Area stringToArea(char* key, char* value);

/**
* areaToString: gets key and value and a area, and puts area id in key and area name in value
*
* @param area - Area area 
* @param key - char** key.
* @param value - char** value.
* @return
*   OUT_OF_MEMORY - if allocations of key or value failed. SUCCESS - if the values where properly put
*/
Result areaToString(Area area, char** key, char** value);

#endif //AREA_H
