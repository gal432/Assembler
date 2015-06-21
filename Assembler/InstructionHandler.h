#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "General.h"
#include "NodesList.h"
#include "Commands.h"
#include "ErrorHandler.h"
#include "AssemblyLine.h"

#define REGISTER0 "r0"
#define REGISTER0_VALUE 0

#define REGISTER1 "r1"
#define REGISTER1_VALUE 1

#define REGISTER2 "r2"
#define REGISTER2_VALUE 2

#define REGISTER3 "r3"
#define REGISTER3_VALUE 3

#define REGISTER4 "r4"
#define REGISTER4_VALUE 4

#define REGISTER5 "r5"
#define REGISTER5_VALUE 5

#define REGISTER6 "r6"
#define REGISTER6_VALUE 6

#define REGISTER7 "r7"
#define REGISTER7_VALUE 7


typedef struct {
	int code;
	int numberOfArguments;
	bool(*correctArgumentsTypes)(NodesList*);
} Command;

typedef struct {
	Command* command;
	NodesList* arguments; /* each node name is the argument it self, and the value is the addressing type */
} Instruction;

typedef struct
{
	NodesList* symbols;
	NodesList* commands;
	NodesList* registers;
	NodesList* instructions;
	NodesList* symbolsUsedInArguments;
	int counter;
	ErrorHolder* errorHolder;
} InstructionsHolder;


InstructionsHolder* CreateInstructions();
Instruction* createInstruction(Command* command, NodesList* arguments);
void freeInstructions(InstructionsHolder* holder);
void freeInstructionsList(NodesList* instructions);

void handleInstruction(InstructionsHolder* holder, AssemblyLine* assemblyLine);
NodesList* extract_command_params(NodesList* arguments, AssemblyLine* assemblyLine, InstructionsHolder* holder);
int getAddressingType(AssemblyLine* assemblyLine, char* argument, InstructionsHolder* holder);
char* getCommandName(char** linePtr);
void addInstructionSymbol(InstructionsHolder* holder, AssemblyLine* assemblyLine);
void addRegisters(NodesList* registers);
void addRegister(NodesList* registers, char* registerName, int registerValue);
void addCommands(NodesList* commands);
void addCommand(NodesList* commands, char* commandName, int code, int numberOfArguments, bool(*checkArguments)(NodesList*));
Command* createCommand(int code, int numberOfArguments, bool(*checkArguments)(NodesList*));

#endif 