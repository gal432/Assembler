#include "GuidelineHandler.h"

GuidelineHolder* CreateGuidelineHolder() {
	GuidelineHolder* holder = (GuidelineHolder*)safeMalloc(sizeof(GuidelineHolder));
	
	holder->symbols = createNodesList(sizeof(int));
	holder->enterySymbols = createNodesList(sizeof(int));
	holder->externSymbols = createNodesList(sizeof(NodesList));
	
	holder->counter = 0;
	holder->errorHolder = createErrorHolder();
	
	return holder;
}

void freeGuidelineHolder(GuidelineHolder* holder) {
	freeNodesList(holder->symbols);
	freeNodesList(holder->enterySymbols);
	freeNodesList(holder->externSymbols);
	freeErrorHolder(holder->errorHolder);
	free(holder);
}

void addData(GuidelineHolder* holder, int data, AssemblyLine* assemblyLine) {
	if (holder->counter < DATA_SIZE) 
	{
		holder->memory[holder->counter] = data;
		holder->counter++;

		return;
	}

	addError(holder->errorHolder, "Data memory is full", assemblyLine->lineNumber);
}

void addDataSymbol(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	if (assemblyLine->labelName)
	{
		if (searchNode(holder->symbols, assemblyLine->labelName))
			addError(holder->errorHolder, "This label already declared", assemblyLine->lineNumber);
	
		else
			addNode(holder->symbols, assemblyLine->labelName, &holder->counter);
	}
}
/*select the relevant handle*/
void handleGuideline(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	if (startsWith(assemblyLine->line, STRING_STR)) {
		handleString(holder, assemblyLine);
	}
	else if (startsWith(assemblyLine->line, DATA_STR)) {
		handleData(holder, assemblyLine);
	}
	else if (startsWith(assemblyLine->line, EXTERN_STR)) {
		handleExtern(holder, assemblyLine);
	}
	else if (startsWith(assemblyLine->line, ENTRY_STR)){
		handleEntern(holder, assemblyLine);
	}
}
/**************************************/
/***************HANDLERS***************/
/**************************************/
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

void handleData(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	addDataSymbol(holder, assemblyLine);
	assemblyLine->line += strlen(DATA_STR);
	assemblyLine->line = trim(assemblyLine->line);
	extractDataParams(holder, assemblyLine);
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
	extractStringParams(holder, assemblyLine);
}

/**************************************/
/****************HELPERS***************/
/**************************************/

void extractDataParams(GuidelineHolder* holder, AssemblyLine* assemblyLine)
{
	const char* delimiters = DELIMATER_PARAMS;
	
	char* convertionEnd;
	int num;
	char* token = strtok(assemblyLine->line, delimiters);
	
	/* walk through the tokens */
	while (token != NULL)
	{
		num = strtol(trim(token), &convertionEnd, 10);
		if ((*convertionEnd)) {
			addError(holder->errorHolder, "Data must contain numbers only", assemblyLine->lineNumber);
			return;
		}
		addData(holder, num, assemblyLine);

		token = strtok(NULL, delimiters);
	}
}

void extractStringParams(GuidelineHolder* holder, AssemblyLine* assemblyLine) {
	char* data = assemblyLine->line;
	while (*data) {
		addData(holder, (int)*data, assemblyLine);
		data++;
	}
	addData(holder, 0, assemblyLine); /* The end of the string */
}

