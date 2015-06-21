#include "General.h"
#include "NodesList.h"

/* Reads the lines from the file */
int readLines(char* fileName, AssemblyLine* lines);

AssemblyLine* createAssemblyLine(char* line, int lineNumber);

char* getLabel(char** linePtr);

/* frees the strings of the lines */
void freeLinesString(NodesList* lines);