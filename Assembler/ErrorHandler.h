#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "General.h"
#include "NodesList.h"

typedef struct
{
	char* message;
	int lineNumber;
} ErrorMessage;

typedef struct
{
	NodesList* errors;
	bool hasError;
} ErrorHolder;

ErrorHolder* createErrorHolder();
void addError(ErrorHolder* errorHolder, char* error, int lineNumber);
ErrorMessage* createErrorMessage(char* error, int lineNumber);
void freeErrorHolder(ErrorHolder* errorHolder);

#endif 