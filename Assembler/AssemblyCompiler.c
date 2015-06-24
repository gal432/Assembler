#include "AssemblyCompiler.h"

void compileAssembly(char* name, NodesList* lines) {
	InformationHolder* holder = createInformationHolder();
	FirstRun(holder, lines);
	checkSymbolsUsedInArguments(holder);
	
	addInstructionsCounterToDataCounter(holder->data->symbols, holder->instructions->counter);
	setEntriesValues(holder);
	
	if (holder->errorHolder->hasError || holder->instructions->errorHolder->hasError || holder->data->errorHolder->hasError)
		printErrors(name, holder);
	else
		SecondRun(name, holder, lines);
	
	freeInformationHolder(holder);
}

void checkSymbolsUsedInArguments(InformationHolder* holder) {
	Node* symbolNode;
	int* lineNum;
	while ((symbolNode = getNext(holder->instructions->symbolsUsedInArguments)))
	{
		/* Find if the symbol exist*/
		Node* holderSymbol = searchNode(holder->data->symbols, symbolNode->name);
		Node* externSymbol = searchNode(holder->data->externSymbols, symbolNode->name);
		Node* insturctionsSymbol = searchNode(holder->instructions->symbols, symbolNode->name);
		
		if (holderSymbol || externSymbol || insturctionsSymbol)
			continue;
		
		lineNum = (int*)(symbolNode->value);
		addError(holder->errorHolder, "The label that was used in the arguments was invalid", *lineNum);
	}
}

void addInstructionsCounterToDataCounter(NodesList* dataSymbols, int counterToAdd)
{
	Node* symbol;

	for (symbol = getNext(dataSymbols); symbol != NULL ; symbol = getNext(dataSymbols))
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

void FirstRun(InformationHolder* holder, NodesList* lines) {
	Node* node;
	AssemblyLine* assemblyLine;
	while ((node = getNext(lines))) {
		assemblyLine = (AssemblyLine*)(node->value);

		/* comment or space doesn't need to handle */
		if (isStringEmpty(assemblyLine->line) || assemblyLine->line[0] == COMMENT_LINE_START)
			continue; 
		
		if (assemblyLine->line[0] == GUIDELINE_LINE_START)
			handleGuideline(holder->data, assemblyLine);
		
		else
			handleInstruction(holder->instructions, assemblyLine);
	}
}

void SecondRun(char* name, InformationHolder* holder, NodesList* lines) {
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