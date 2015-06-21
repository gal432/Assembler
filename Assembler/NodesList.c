#include "NodesList.h"

NodesList* createNodesList(int elementSize) {
	NodesList* list = (NodesList*)safeMalloc(sizeof(NodesList));
	list->head = NULL;
	list->tail = NULL;
	list->current = NULL;
	list->elementSize = elementSize;
	list->length = 0;
	return list;
}

Node* searchNode(NodesList* nodesList, char* name) {
	Node* currentNode = nodesList->head;

	while (currentNode)
	{
		if (strcmp(currentNode->name, name) == 0)
			return currentNode;
		currentNode = currentNode->next;
	}
	return NULL;
}

void addNode(NodesList* nodesList, char* name, void* value) {
	Node* node = createNode(name, value, nodesList->elementSize);
	if (nodesList->length == 0)
		nodesList->head = (nodesList->tail = node);
	else {
		nodesList->tail->next = node;
		nodesList->tail = node;
	}
	nodesList->length++;
}

Node* createNode(char* name, void* value, int elementSize) {
	Node* node = (Node*)safeMalloc(sizeof(Node));
	node->name = name;
	if (NULL != value) {
		node->value = safeMalloc(elementSize);
		memcpy(node->value, value, elementSize);
		node->hasValue = TRUE;
	}
	else
		node->hasValue = FALSE;
	node->next = NULL;
	return node;
}

Node* getNext(NodesList* nodesList) {
	if (NULL == nodesList->current)
		nodesList->current = nodesList->head;
	else
		nodesList->current = nodesList->current->next;
	return nodesList->current;
}

void freeNodesList(NodesList* nodesList) {
	Node* currentNode = nodesList->head;
	Node* tempNode;
	while (currentNode)
	{
		tempNode = currentNode;
		currentNode = currentNode->next;
		if (tempNode->hasValue)
			free(tempNode->value);
		free(tempNode);
	}
	free(nodesList);
}