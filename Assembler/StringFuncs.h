#ifndef STRINGS_FUNCS_H
#define STRINGS_FUNCS_H

#include <string.h>
#include <ctype.h>

#include "General.h"
#include "InstructionHandler.h"


/* Removes the white spaces in a string */
char* trim(char *str);

/* Seperates the string with first appearance of the delimiter */
char* seperateString(char** str, char delimiter);

bool startsWith(char* source, char* find);

bool isStringEmpty(char* str);

#endif
