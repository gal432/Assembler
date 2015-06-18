#include <stdlib.h>
#include "InformationHolder.h"
#include "NodesList.h"
#include "General.h"

InformationHolder* createInformationHolder() {
	InformationHolder* holder = (InformationHolder*)safeMalloc(sizeof(InformationHolder));
	holder->data = CreateGuidelineHolder();
	holder->instructions = CreateInstructions();
	holder->errorHolder = createErrorHolder();
	return holder;
}

void freeInformationHolder(InformationHolder* holder) {
	freeGuidelineHolder(holder->data);
	freeInstructions(holder->instructions);
	freeErrorHolder(holder->errorHolder);
	free(holder);
}