#include "StringFuncs.h"

/*Removes the white spaces in a string */
char* trim(char *str)
{
	char *end;
	/* Trim leading space */
	while (*str != '\"' && isspace((unsigned char)*str)) str++;

	if (*str == 0)  /* All spaces */
		return str;

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	/* Write new null terminator */
	*(end + 1) = 0;

	return str;
}

char* seperateString(char** str, char delimiter) {
	char* innerString = *str;
	char *delimiterPtr = strchr(*str, delimiter);
	if (delimiterPtr) {
		int delimiterIndex = delimiterPtr - *str;
		*(innerString + delimiterIndex) = 0; /* Make the delimiter as the end of the inner string */
		*str += delimiterIndex + 1;
		return innerString;
	}

	return NULL;
}

bool startsWith(char* source, char* find) {
	return !strncmp(source, find, strlen(find));
}

bool isStringEmpty(char* str)
{
	return strlen(str) == 0;
}