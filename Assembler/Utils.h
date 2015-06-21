#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void* safeMalloc(size_t size);
int getValueAsInt(void* value);
char* getLabel(char** linePtr);
#endif