#include <stdio.h>
#include "NodesList.h"

#define OBJECT_EXTENSION "ob"
#define ENTRIES_EXTENSION "ent"
#define EXTRENALS_EXTENSION "ext"

void createObjectFile(char* name, NodesList* words, int instructsCounter, int dataCounter);

void createEntriesFile(char* name, NodesList* entrySymbols);

void createExtrenalsFile(char* name, NodesList* externSymbols);

FILE* createOutputFile(char* name, char* fileExtension);