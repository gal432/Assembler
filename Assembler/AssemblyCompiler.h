#ifndef ASSEMBLY_COMPILER_H
#define ASSEMBLY_COMPILER_H

#include "InformationHolder.h"
#include "General.h"
#include "NodesList.h"

#define COMMENT_LINE_START ';'
#define GUIDELINE_LINE_START '.'

void compileAssembly(char* name, NodesList* lines);
void handleFirstRun(InformationHolder* holder, NodesList*);
void handleSecondRun(char* name, InformationHolder* holder, NodesList*);

/* Checks if the symbols that were used in the arguments exist */
void checkSymbolsUsedInArguments(InformationHolder* holder);

/* adds the number of instructions to each address of the data symbols */
void addInstructionsCounterToDataCounter(NodesList* dataSymbols, int counterToAdd);

/* Sets for each entry the address where it was defined */
void setEntriesValues(InformationHolder* holder);

void printErrors(char* name, InformationHolder* holder);

void PrintError(Node* errorNode);

#endif