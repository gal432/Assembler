#ifndef STRINGS_FUNCS_H
#define STRINGS_FUNCS_H

#include "General.h"

/* Removes the white spaces in a string */
char* trim(char *str);

/* Seperates the string with first appearance of the delimiter */
char* seperateString(char** str, char delimiter);

bool startsWith(char* source, char* find);

bool isStringEmpty(char* str);

#endif