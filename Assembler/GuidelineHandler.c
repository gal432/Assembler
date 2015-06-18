#include <string.h>
#include <stdlib.h>
#include "GuidelineHandler.h"
#include "StringFuncs.h"

GuidelineHolder* CreateGuidelineHolder() {
	GuidelineHolder* holder = (GuidelineHolder*)safeMalloc(sizeof(GuidelineHolder));
	holder->symbols = createNodesList(sizeof(int));
	holder->externSymbols = createNodesList(sizeof(NodesList));
	holder->enterySymbols = createNodesList(sizeof(int));
	holder->counter = 0;
	holder->errorHolder = createErrorHolder();
	return holder;
}

void freeGuidelineHolder(GuidelineHolder* holder) {
	freeNodesList(holder->symbols);
	freeNodesList(holder->externSymbols);
	freeNodesList(holder->enterySymbols);
	freeErrorHolder(holder->errorHolder);
	free(holder);
}

void addData(GuidelineHolder* holder, int data, AssemblyLine* assemblyLine) {
	if (holder->counter == DATA_SIZE) {
		addError(holder->errorHolder, "Data memory is full", assemblyLine->lineNumber);
	}
	else
	{
		holder->memory[holder->counter] = data;
		holder->counter++;
	}
}

void addDataSymbol(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	if (assemblyLine->labelName) {
		if (searchNode(holder->symbols, assemblyLine->labelName))
			addError(holder->errorHolder, "This label was already declared", assemblyLine->lineNumber);
		else
			addNode(holder->symbols, assemblyLine->labelName, &holder->counter);
	}
}

void handleGuideline(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	/*If symbol is .data*/ 
	if (startsWith(assemblyLine->line, DATA_STR)) {
		handleData(holder, assemblyLine);
	}
	else if (startsWith(assemblyLine->line, STRING_STR)) {
		handleString(holder, assemblyLine);
	}
	else if (startsWith(assemblyLine->line, ENTRY_STR)){
		handleEntern(holder, assemblyLine);
	}
	else if (startsWith(assemblyLine->line, EXTERN_STR)) {
		handleExtern(holder, assemblyLine);
	}
}

void handleData(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	addDataSymbol(holder, assemblyLine);
	assemblyLine->line += strlen(DATA_STR);
	assemblyLine->line = trim(assemblyLine->line);
	extract_data_params(holder, assemblyLine);
}

void extract_data_params(GuidelineHolder* holder, AssemblyLine* assemblyLine)
{
	const char* delimiters = ",";
	char* token;
	char* convertionEnd;
	int number;

	/* walk through the tokens */
	for (token = strtok(assemblyLine->line, delimiters); token != NULL; token = strtok(NULL, delimiters))
	{
		number = strtol(trim(token), &convertionEnd, 10);
		if ((*convertionEnd)) {
			addError(holder->errorHolder, "Data must contain numbers only", assemblyLine->lineNumber);
			return;
		}
		addData(holder, number, assemblyLine);
	}
}

void handleString(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	addDataSymbol(holder, assemblyLine);
	assemblyLine->line += strlen(STRING_STR);
	assemblyLine->line = trim(assemblyLine->line);	
	if (assemblyLine->line[0] != '\"' || assemblyLine->line[strlen(assemblyLine->line) - 1] != '\"') {
		addError(holder->errorHolder, "String data must be between quotation marks", assemblyLine->lineNumber);
		return;
	}
	*(assemblyLine->line + strlen(assemblyLine->line) - 1) = '\0';
	assemblyLine->line++;
	extract_string_params(holder, assemblyLine);
}

void extract_string_params(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	char* data = assemblyLine->line;
	while (*data) {
		addData(holder, (int)*data, assemblyLine);
		data++;
	}
	addData(holder, 0, assemblyLine); /* The end of the string */
}

void handleExtern(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	NodesList* nodeslist;
	assemblyLine->line += strlen(EXTERN_STR);
	assemblyLine->line = trim(assemblyLine->line);
	if (assemblyLine->line) {
		nodeslist = createNodesList(sizeof(int));
		addNode(holder->externSymbols, assemblyLine->line, nodeslist);
	}
}


void handleEntern(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	assemblyLine->line += strlen(ENTRY_STR);
	assemblyLine->line = trim(assemblyLine->line);
	if (assemblyLine->line)
		addNode(holder->enterySymbols, assemblyLine->line, &(assemblyLine->lineNumber));
}