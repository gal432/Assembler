#include "AssemblyCompiler.h"

void compileAssembly(char* name, NodesList* lines) {
	InformationHolder* holder = createInformationHolder();
	handleFirstRun(holder, lines);
	checkSymbolsUsedInArguments(holder);
	
	addInstructionsCounterToDataCounter(holder->data->symbols, holder->instructions->counter);
	setEntriesValues(holder);
	
	if (holder->errorHolder->hasError || holder->instructions->errorHolder->hasError || holder->data->errorHolder->hasError)
		printErrors(name, holder);
	else
		handleSecondRun(name, holder, lines);
	
	freeInformationHolder(holder);
}

void checkSymbolsUsedInArguments(InformationHolder* holder) {
	Node* symbolNode;
	int* lineNumber;
	while ((symbolNode = getNext(holder->instructions->symbolsUsedInArguments))) {
		if (searchNode(holder->data->symbols, symbolNode->name) || searchNode(holder->data->externSymbols, symbolNode->name) || searchNode(holder->instructions->symbols, symbolNode->name))
			continue;
		lineNumber = (int*)(symbolNode->value);
		addError(holder->errorHolder, "The label that was used in the arguments was invalid", *lineNumber);
	}
}

void addInstructionsCounterToDataCounter(NodesList* dataSymbols, int counterToAdd)
{
	Node* symbol;
	while ((symbol = getNext(dataSymbols)))
	{
		int* address = (int*)symbol->value;
		*address += counterToAdd;
	}
}

void setEntriesValues(InformationHolder* holder)
{
	Node* entryNode;
	Node* symbol;
	while ((entryNode = getNext(holder->data->enterySymbols)))
	{
		if ((symbol = searchNode(holder->instructions->symbols, entryNode->name)))
		{
			*((int*)(entryNode->value)) = *((int*)(symbol->value));
		}
		else if ((symbol = searchNode(holder->data->symbols, entryNode->name)))
		{
			*((int*)(entryNode->value)) = *((int*)(symbol->value));
		}
		else
		{
			addError(holder->errorHolder, "The entry wasn't defined", *((int*)(entryNode->value)));
		}
	}
}

void handleFirstRun(InformationHolder* holder, NodesList* lines) {
	Node* node;
	AssemblyLine* assemblyLine;
	while ((node = getNext(lines))) {
		assemblyLine = (AssemblyLine*)(node->value);
		if (isStringEmpty(assemblyLine->line) || assemblyLine->line[0] == COMMENT_LINE_START)
			continue; /* Empty line or comment line */
		if (assemblyLine->line[0] == GUIDELINE_LINE_START)
			handleGuideline(holder->data, assemblyLine);
		else
			handleInstruction(holder->instructions, assemblyLine);
	}
}

void handleSecondRun(char* name, InformationHolder* holder, NodesList* lines) {
	NodesList* words = translateInforamtion(holder->instructions, holder->data, holder->data->symbols, holder->data->externSymbols);
	createObjectFile(name, words, holder->instructions->counter, holder->data->counter);
	createEntriesFile(name, holder->data->enterySymbols);
	createExtrenalsFile(name, holder->data->externSymbols);
	freeNodesList(words);
}

void printErrors(char* name, InformationHolder* holder) {
	Node* errorNode;
	printf("Errors in the file %s.as:\n\n", name);
	while ((errorNode = getNext(holder->errorHolder->errors)))
		PrintError(errorNode);
	while ((errorNode = getNext(holder->data->errorHolder->errors)))
		PrintError(errorNode);
	while ((errorNode = getNext(holder->instructions->errorHolder->errors)))
		PrintError(errorNode);
	printf("\n\n");
}

void PrintError(Node* errorNode) {
	ErrorMessage* errorMessage = (ErrorMessage*)(errorNode->value);
	printf("Line number %d: %s\n", errorMessage->lineNumber, errorMessage->message);
}