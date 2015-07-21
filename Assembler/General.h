#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include "StringFuncs.h"

#define MEMORY_ADDRESS_START 100
#define MAX_LINE_LEN 80
#define FILE_NAME_SIZE 25

#define ENCODING_BITS_SIZE 2
#define DESTINATION_BITS_SIZE 2
#define SOURCE_BITS_SIZE 2
#define OPCODE_BITS_SIZE 4
#define GROUP_BITS_SIZE 2

/* A,R,E encoding values */
#define ABSOLUTE_ENCODING 0
#define RELOCATABLE_ENCODING 2
#define EXTERNAL_ENCODING 1

#define BITS_TO_ZERO_START 0xFFF

#define ALL_BITS_SIZE (ENCODING_BITS_SIZE+DESTINATION_BITS_SIZE+SOURCE_BITS_SIZE+OPCODE_BITS_SIZE+GROUP_BITS_SIZE)

typedef enum { FALSE, TRUE } bool;

void* safeMalloc(size_t size);
int getValueAsInt(void* value);
char* getLabel(char** linePtr);

#endif
