#ifndef ASSEMBLY_LINE_H
#define ASSEMBLY_LINE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _assemblyLine {
	char* originalLinePtr;
	char* labelName;
	char* line;
	int lineNumber;
} AssemblyLine;

AssemblyLine* createAssemblyLine(char* line, int lineNumber);


#endif
