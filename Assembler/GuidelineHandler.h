#ifndef GUIDELINE_HANLER_H
#define GUIDELINE_HANLER_H

#include "General.h"
#include "NodesList.h"
#include "ErrorHandler.h"
#include "Word.h"
#include "AssemblyLine.h"

#define DATA_SIZE 1000

#define DATA_STR ".data"
#define STRING_STR ".string"
#define ENTRY_STR ".entry"
#define EXTERN_STR ".extern"

typedef struct
{
	NodesList* symbols;
	NodesList* externSymbols;
	NodesList* enterySymbols;
	int memory[DATA_SIZE];
	int counter;
	ErrorHolder* errorHolder;
} GuidelineHolder;

GuidelineHolder* CreateGuidelineHolder();
void freeGuidelineHolder(GuidelineHolder* holder);

void addData(GuidelineHolder* holder, int data, AssemblyLine* assemblyLine);
void addDataSymbol(GuidelineHolder* holder, AssemblyLine* assemblyLine);

void handleGuideline(GuidelineHolder* holder, AssemblyLine* assemblyLine);
void handleData(GuidelineHolder* holder, AssemblyLine* assemblyLine);
void extract_data_params(GuidelineHolder* holder, AssemblyLine* assemblyLine);
void handleString(GuidelineHolder* holder, AssemblyLine* assemblyLine);
void extract_string_params(GuidelineHolder* holder, AssemblyLine* assemblyLine);
void handleExtern(GuidelineHolder* holder, AssemblyLine* assemblyLine);
void handleEntern(GuidelineHolder* holder, AssemblyLine* assemblyLine);

#endif