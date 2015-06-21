#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Assembler.h"
#include "AssemblyCompiler.h"
#include "StringFuncs.h"
#define DEBUG 1


int main(int argc, char* argv[])
{
	AssemblyLine lines[50];

	if (1 == DEBUG)
	{
		char* fileName;
		int i;
		char assemblyFileName[FILE_NAME_SIZE];
		
		fileName = "C:\\Temp\\ps.as";
		//sprintf(assemblyFileName, "%s.as", fileName);
		
		int numOfLines = readLines(fileName, &lines);
		compileAssembly(fileName, &lines);
		//freeLinesString(lines);
		//freeNodesList(lines);
		return 0;
	}

	/*
	char* fileName;
	int i;
	char assemblyFileName[FILE_NAME_SIZE];
	
	for (i = 1; i < argc; i++) {
		fileName = argv[i];
		sprintf(assemblyFileName, "%s.as", fileName);
		int linesNumber = readLines(assemblyFileName, lines);
		compileAssembly(fileName, lines);
		freeLinesString(lines);
		freeNodesList(lines);
	}*/
	return 0;
}

int readLines(char* fileName, AssemblyLine* lines) {
	char currentLine[MAX_LINE_LEN];
	AssemblyLine* assemblyLine;
	int lineNumber = 0;

	FILE* file = fopen(fileName, "r");

	if (file == NULL)
	{
		printf("Error: cant open file %s\n", fileName);
		exit(0);
	}

	while (fgets(currentLine, MAX_LINE_LEN, file)){
		assemblyLine = createAssemblyLine(currentLine, ++lineNumber);
		assemblyLine->line = trim(assemblyLine->line);
		assemblyLine->labelName = getLabel(&(assemblyLine->line));
		assemblyLine->line = trim(assemblyLine->line); /* After getting the label name, we need to trim again */
		
		lines[lineNumber - 1] = (*assemblyLine);
		//addNode(linesList, NULL, assemblyLine);
		free(assemblyLine); /* when we add the node, we copy the bits to a new memory block, so we can free this line */
	}
	fclose(file);

	return lineNumber -1;
}

AssemblyLine* createAssemblyLine(char* line, int lineNumber) {
	AssemblyLine* assemblyLine = (AssemblyLine*)safeMalloc(sizeof(AssemblyLine));
	char* newLine = (char*)safeMalloc( sizeof(char) * (strlen(line) + 1) );
	memcpy(newLine, line, sizeof(char)* (strlen(line) + 1) );
	assemblyLine->originalLinePtr = newLine;
	assemblyLine->line = newLine;
	assemblyLine->lineNumber = lineNumber;
	return assemblyLine;
}

char* getLabel(char** linePtr) {
	const char labelDelimiter = ':';
	return seperateString(linePtr, labelDelimiter);
}