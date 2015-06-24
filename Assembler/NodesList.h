#ifndef NODES_LIST_H
#define NODES_LIST_H

#include <stdlib.h>
#include <string.h>

#include "General.h"

/* This is a generic linked list
the data value is saved with a (void*) variable and the size of data is given when creating the list
when we need to read the value, we cast it to the proper type(int*, Command* and etc)
*/
typedef struct _node
{
	char* name;
	void* value;
	struct _node* next;
} Node;

typedef struct _NodesList
{
	Node* head;
	Node* tail;
	Node* current;
	int elementSize;
	int length;
} NodesList;

NodesList* createNodesList(int elementSize);
Node* searchNode(NodesList* nodesList, char* name);
void addNode(NodesList* nodesList, char* name, void* value);
Node* createNode(char* name, void* value, int elementSize);
Node* getNext(NodesList* nodesList);
void freeNodesList(NodesList* nodesList);

#endif