#include "AssemblyLine.h"

AssemblyLine* createAssemblyLine(char* line, int lineNumber) {
	AssemblyLine* assLine = (AssemblyLine*)safeMalloc(sizeof(AssemblyLine));
	char* newLine = (char*)safeMalloc(sizeof(char) * (strlen(line) + 1));

	memcpy(newLine, line, sizeof(char)* (strlen(line) + 1));

	assLine->line = newLine;
	assLine->originalLinePtr = newLine;
	assLine->lineNumber = lineNumber;
	return assLine;
}

