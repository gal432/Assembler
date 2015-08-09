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
	fprintf(objectFile, "%u %u\n", convertToBase4(instructsCounter - MEMORY_ADDRESS_START), convertToBase4(dataCounter));
	while ((wordNode = getNext(words)))
	{
		wordHolder = (WordHolder*)(wordNode->value);
		fprintf(objectFile, "%u %06u\n", convertToBase4(wordHolder->address), convertToBase4(wordHolder->value));
	}
	fclose(objectFile);
}

void createEntriesFile(char* name, NodesList* entrySymbols)
{
	Node* symbolNode;
	if (entrySymbols->length > 0)
	{
		FILE* entriesFile = createOutputFile(name, ENTRIES_EXTENSION);
		while ((symbolNode = getNext(entrySymbols)))
		{
			fprintf(entriesFile, "%s %u\n", symbolNode->name, convertToBase4(getValueAsInt(symbolNode->value)));
		}
		fclose(entriesFile);
	}
}

void createExtrenalsFile(char* name, NodesList* externSymbols)
{
	Node* symbolNode;
	NodesList* addressesList;
	Node* addressNode;

	if (externSymbols->length>0)
	{
		FILE* extrenalsFile = createOutputFile(name, EXTRENALS_EXTENSION);
		while ((symbolNode = getNext(externSymbols)))
		{
			addressesList = (NodesList*)(symbolNode->value);
			while ((addressNode = getNext(addressesList)))
			{
				fprintf(extrenalsFile, "%s %u\n", symbolNode->name, convertToBase4(getValueAsInt(addressNode->value)));
			}
		}
		fclose(extrenalsFile);
	}
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