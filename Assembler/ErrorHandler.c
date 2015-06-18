#include <stdio.h>
#include "ErrorHandler.h"

ErrorHolder* createErrorHolder() {
	ErrorHolder* errorHolder = (ErrorHolder*)safeMalloc(sizeof(ErrorHolder));
	errorHolder->errors = createNodesList(sizeof(ErrorMessage));
	errorHolder->hasError = FALSE;
	return errorHolder;
}

void addError(ErrorHolder* errorHolder, char* error, int lineNumber) {
	ErrorMessage* errorMessage;
	errorMessage = createErrorMessage(error, lineNumber);
	errorHolder->hasError = TRUE;
	addNode(errorHolder->errors, NULL, errorMessage);
	free(errorMessage);
}

ErrorMessage* createErrorMessage(char* error, int lineNumber) {
	ErrorMessage* errorMessage = (ErrorMessage*)safeMalloc(sizeof(ErrorMessage));
	errorMessage->message = error;
	errorMessage->lineNumber = lineNumber;
	return errorMessage;
}

void freeErrorHolder(ErrorHolder* errorHolder) {
	freeNodesList(errorHolder->errors);
	free(errorHolder);
}