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

		lines = readLinesToAssemblyList(fileName);
		compileAssembly(fileName, lines);
		freeLinesString(lines);
		freeNodesList(lines);
		return 0;
	}

	/*
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
	return 0;*/
}

NodesList* readLinesToAssemblyList(char* fileName) {
	char currentLine[MAX_LINE_LEN];
	AssemblyLine* assemblyLine;
	int lineNumber = 0;
	NodesList* linesList = createNodesList(sizeof(AssemblyLine));

	FILE* file = fopen(fileName, "r");

	if (NULL == file)
	{
		printf("Error: Couldn't open the file %s\n", fileName);
		exit(0);
	}

	while (fgets(currentLine, MAX_LINE_LEN, file)){
		assemblyLine = createAssemblyLine(currentLine, ++lineNumber);
		assemblyLine->line = trim(assemblyLine->line);
		
		assemblyLine->labelName = getLabel(&(assemblyLine->line));
		
		/* After getting the label name, we need to trim again */
		assemblyLine->line = trim(assemblyLine->line);
		addNode(linesList, NULL, assemblyLine);
		
		free(assemblyLine); /* when we add the node, we copy the bits to a new memory block, so we can free this line */
	}
	fclose(file);

	return linesList;
}

void freeLinesString(NodesList* lines) {
	Node* node;
	while ((node = getNext(lines))) {
		free(((AssemblyLine*)(node->value))->originalLinePtr);
	}
}