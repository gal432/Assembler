#include "OutputCreator.h"

int convertToBase4(int src)
{
	if (0 == src)
		return src;

	return (src % 4) + 10 * convertToBase4(src / 4);
}


void createObjectFile(char* name, NodesList* words, int instructsCounter, int dataCounter)
{
	Node* wordNode;
	WordHolder* wordHolder;
	FILE* objectFile = createOutputFile(name, OBJECT_EXTENSION);
	fprintf(objectFile, "%X %X\n", instructsCounter - MEMORY_ADDRESS_START, dataCounter);
	while ((wordNode = getNext(words)))
	{
		wordHolder = (WordHolder*)(wordNode->value);
		fprintf(objectFile, "%X %03X\n", wordHolder->address, wordHolder->value);
	}
	fclose(objectFile);
}

void createEntriesFile(char* name, NodesList* entrySymbols)
{
	Node* symbolNode;
	FILE* entriesFile = createOutputFile(name, ENTRIES_EXTENSION);
	while ((symbolNode = getNext(entrySymbols)))
	{
		fprintf(entriesFile, "%s %X\n", symbolNode->name, getValueAsInt(symbolNode->value));
	}
	fclose(entriesFile);
}

void createExtrenalsFile(char* name, NodesList* externSymbols)
{
	Node* symbolNode;
	NodesList* addressesList;
	Node* addressNode;
	FILE* extrenalsFile = createOutputFile(name, EXTRENALS_EXTENSION);
	while ((symbolNode = getNext(externSymbols)))
	{
		addressesList = (NodesList*)(symbolNode->value);
		while ((addressNode = getNext(addressesList)))
		{
			fprintf(extrenalsFile, "%s %X\n", symbolNode->name, getValueAsInt(addressNode->value));
		}		
	}
	fclose(extrenalsFile);
}

FILE* createOutputFile(char* name, char* fileExtension)
{
	FILE* file;
	char fileName[FILE_NAME_SIZE];
	sprintf(fileName, "%s.%s", name, fileExtension);
	file = fopen(fileName, "w");
	if (!file)
	{
		printf("Couldn't create a file\n");
		exit(0);
	}
	return file;
}