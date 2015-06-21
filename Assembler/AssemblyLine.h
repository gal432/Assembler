#ifndef ASSEMBLY_LINE_H
#define ASSEMBLY_LINE_H

typedef struct _assemblyLine {
	char* originalLinePtr;
	char* labelName;
	char* line;
	int lineNumber;
} AssemblyLine;
#endif