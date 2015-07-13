#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "StringFuncs.h"

void* safeMalloc(size_t size);
int getValueAsInt(void* value);
char* getLabel(char** linePtr);
#endif