#ifndef INFORMATION_HOLDER_H
#define INFORMATION_HOLDER_H

#include "NodesList.h"
#include "InstructionHandler.h"
#include "GuidelineHandler.h"
#include "ErrorHandler.h"

typedef struct
{
	InstructionsHolder* instructions;
	GuidelineHolder* data;
	ErrorHolder* errorHolder;
} InformationHolder;

InformationHolder * createInformationHolder();
void freeInformationHolder(InformationHolder* holder);

#endif