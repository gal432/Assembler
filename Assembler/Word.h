#ifndef WORD_H
#define WORD_H
#include "General.h"

typedef struct{
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

#endif