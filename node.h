#ifndef NODE_H_
#define NODE_H_

//Decleration of the struct node*
typedef struct node *Node;

/**
* nodeCreate: Allocates a new node.
* @param key - the key of the node
* @param value - the vlue of the node
* @return
* 	NULL - if allocations failed
* 	A new Node in case of success.
*/
Node nodeCreate(const char* key, const char* value, Node next);

/**
* nodeDestroy: Deallocates an existing node.
*
* @param node - Target node to be deallocated. If node is NULL nothing will be
* 		done
*/
void nodeDestroy(Node node);

/**
* nodeListDestroy: Deallocates an existing node list.
*
* @param node - Target node list head to be deallocated. If head is NULL nothing will be
* 		done
*/
void nodeListDestroy(Node head);

/**
* nodeGetKey: returns the key of a specific node
*
* @param node - Target node.
* @return
*   key of the node 
*/
char* nodeGetKey(Node node);

/**
* nodeSetKey: puts the given key in a specific node
*
* @param node - Target node.
* @param key - key to put in node.
*/
void nodeSetKey(Node node, char* key);

/**
* nodeGetValue: returns the value of a specific node
*
* @param node - Target node.
* @return
*   value of the node 
*/
char* nodeGetValue(Node node);

/**
* nodeSetValue: puts the given value in a specific node
*
* @param node - Target node.
* @param value - value to put in node.
*/
void nodeSetValue(Node node, char* value);

/**
* nodeGetNext: returns the next node of a specific node
*
* @param node - Target node.
* @return
*   next node of the node 
*/
Node nodeGetNext(Node node);

/**
* nodeSetNext: puts the given next node in a specific node
*
* @param node - Target node.
* @param next - next node to put in node.
*/
void nodeSetNext(Node node, Node next);

/**
* nodeCopyData: Creates a copy of target node.
*
* @param node - Target node.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Node containing the same elements as node otherwise.
*/
Node nodeCopyData(Node node);

void nodePrint(Node node);

#endif // NODE_H_
