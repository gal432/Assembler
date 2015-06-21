#include "Assembler.h"


#define DEBUG 1


int main(int argc, char* argv[])
{
	if (1 == DEBUG)
	{
		char* fileName;
		int i;
		char assemblyFileName[FILE_NAME_SIZE];
		NodesList* lines = NULL;

		fileName = "C:\\Temp\\ps.as";
		//sprintf(assemblyFileName, "%s.as", fileName);

		lines = readLines(fileName);
		compileAssembly(fileName, lines);
		freeLinesString(lines);
		freeNodesList(lines);


		return 0;
	}


	char* fileName;
	int i;
	char assemblyFileName[FILE_NAME_SIZE];
	NodesList* lines = NULL;
	for (i = 1; i < argc; i++) {
		fileName = argv[i];
		sprintf(assemblyFileName, "%s.as", fileName);
		lines = readLines(assemblyFileName);
		compileAssembly(fileName, lines);
		freeLinesString(lines);
		freeNodesList(lines);
	}
	return 0;
}

NodesList* readLines(char* fileName) {
	char currentLine[MAX_LINE_LEN];
	AssemblyLine* assemblyLine;
	int lineNumber = 0;
	NodesList* linesList = createNodesList(sizeof(AssemblyLine));

	FILE* file = fopen(fileName, "r");

	if (file == NULL)
	{
		printf("Error: Couldn't open the file %s\n", fileName);
		exit(0);
	}

	while (fgets(currentLine, MAX_LINE_LEN, file)){
		assemblyLine = createAssemblyLine(currentLine, ++lineNumber);
		assemblyLine->line = trim(assemblyLine->line);
		assemblyLine->labelName = getLabel(&(assemblyLine->line));
		assemblyLine->line = trim(assemblyLine->line); /* After getting the label name, we need to trim again */
		addNode(linesList, NULL, assemblyLine);
		free(assemblyLine); /* when we add the node, we copy the bits to a new memory block, so we can free this line */
	}
	fclose(file);

	return linesList;
}

AssemblyLine* createAssemblyLine(char* line, int lineNumber) {
	AssemblyLine* assemblyLine = (AssemblyLine*)safeMalloc(sizeof(AssemblyLine));
	char* newLine = (char*)safeMalloc(sizeof(char) * (strlen(line) + 1));
	memcpy(newLine, line, sizeof(char)* (strlen(line) + 1));
	assemblyLine->originalLinePtr = newLine;
	assemblyLine->line = newLine;
	assemblyLine->lineNumber = lineNumber;
	return assemblyLine;
}

char* getLabel(char** linePtr) {
	const char labelDelimiter = ':';
	return seperateString(linePtr, labelDelimiter);
}

void freeLinesString(NodesList* lines) {
	Node* node;
	while ((node = getNext(lines))) {
		free(((AssemblyLine*)(node->value))->originalLinePtr);
	}
}