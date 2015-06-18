#include "General.h"
#include "NodesList.h"

/* Reads the lines from the file */
NodesList* readLines(char* fileName);

AssemblyLine* createAssemblyLine(char* line, int lineNumber);

char* getLabel(char** linePtr);

/* frees the strings of the lines */
void freeLinesString(NodesList* lines);