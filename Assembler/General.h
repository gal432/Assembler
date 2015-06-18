#ifndef GENERAL_H
#define GENERAL_H

#include <stdlib.h>

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

typedef struct _assemblyLine {
	char* originalLinePtr;
	char* labelName;
	char* line;
	int lineNumber;
} AssemblyLine;

typedef struct {
	unsigned int encodingType : ENCODING_BITS_SIZE;
	unsigned int destination : DESTINATION_BITS_SIZE;
	unsigned int source : SOURCE_BITS_SIZE;
	unsigned int opcode : OPCODE_BITS_SIZE;
	unsigned int group : GROUP_BITS_SIZE;
} FirstWord;

typedef struct {
	unsigned int encodingType : ENCODING_BITS_SIZE;
	int value : (ALL_BITS_SIZE - ENCODING_BITS_SIZE);
} ArgumentWord;

typedef struct {
	unsigned int encodingType : ENCODING_BITS_SIZE;
	int right_value : (ALL_BITS_SIZE - ENCODING_BITS_SIZE) / 2;
	int left_value : (ALL_BITS_SIZE - ENCODING_BITS_SIZE) / 2;
} RegisterArgumentWord;

typedef struct {
	int value : ALL_BITS_SIZE;
} DataWord;

typedef struct {
	int address;
	int value;
} WordHolder;

void* safeMalloc(size_t size);
int getValueAsInt(void* value);

#endif