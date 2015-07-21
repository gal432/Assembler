#include "General.h"

void* safeMalloc(size_t size) {
	void* toMalloc = malloc(size);
	if (!toMalloc) {
		printf("Coudn't allocate memory\n");
		exit(0);
	}
	return toMalloc;
}

int getValueAsInt(void* value)
{
	return *((int*)value);
}

char* getLabel(char** linePtr) {
	const char labelDelimiter = ':';
	return seperateString(linePtr, labelDelimiter);
}
