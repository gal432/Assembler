#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "General.h"
#include "NodesList.h"
#include "Commands.h"
#include "ErrorHandler.h"
#include "AssemblyLine.h"

#define REG0 "r0"
#define REG0_VALUE 0

#define REG1 "r1"
#define REG1_VALUE 1

#define REG2 "r2"
#define REG2_VALUE 2

#define REG3 "r3"
#define REG3_VALUE 3

#define REG4 "r4"
#define REG4_VALUE 4

#define REG5 "r5"
#define REG5_VALUE 5

#define REG6 "r6"
#define REG6_VALUE 6

#define REG7 "r7"
#define REG7_VALUE 7


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