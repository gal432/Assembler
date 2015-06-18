#include <stdio.h>
#include "General.h"

void* safeMalloc(size_t size) {
	void* instance = malloc(size);
	if (!instance) {
		printf("Coudn't allocate memory\n");
		exit(0);
	}
	return instance;
}

int getValueAsInt(void* value)
{
	return *((int*)value);
}

