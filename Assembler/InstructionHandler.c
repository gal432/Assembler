#include "InstructionHandler.h"

void handleInstruction(InstructionsHolder* holder, AssemblyLine* assemblyLine, char** lastArgument) {
	int instructionLength;
	int destination;
	int source;
	int zeroArguments;
	int returnTimes;
	char* commandName;
	char* orginalLine;
	bool success;
	Command* command;
	NodesList* arguments;
	
	zeroArguments = FALSE;
	source = 0;
	destination = 0;
	instructionLength = 1;
	success = TRUE;

	addInstructionSymbol(holder, assemblyLine);
	orginalLine = assemblyLine->line;
	
	commandName = getCommandName(&(assemblyLine->line));
	
	/* Command without arguments */
	if (!commandName) {
		commandName = assemblyLine->line; 
		zeroArguments = TRUE;
	}
	command = searchCommand(holder->commands, commandName);

	if (NULL == command) {
		addError(holder->errorHolder, "Command name is invalid", assemblyLine->lineNumber);
		return;
	}
	arguments = createNodesList(sizeof(int));
	
	returnTimes = extractCommandReturnTimes(orginalLine);
	if (1 != returnTimes && 2 != returnTimes)
	{
		addError(holder->errorHolder, "number of commands is invalid", assemblyLine->lineNumber);
		return;
	}
	command->numOfReturnTimes = returnTimes;

	/*extract parameters*/
	if (!zeroArguments)
	{
		success = extractCommandParams(arguments, assemblyLine, holder, lastArgument);
		if (!success)
		{
			*lastArgument = NULL;
			return;
		}
	}
		
	if (arguments->length != command->numberOfArguments) {
		addError(holder->errorHolder, "Number of command arguments was wrong", assemblyLine->lineNumber);
		*lastArgument = NULL;
		return;
	}

	if (!command->correctArgumentsTypes(arguments)) {
		addError(holder->errorHolder, "Wrong type of arguments", assemblyLine->lineNumber);
		*lastArgument = NULL;
		return;
	}
	
	if (arguments->length == 1 || arguments->length == 2)
	{
		*lastArgument = arguments->head->name;
	}
	else
	{
		*lastArgument = NULL;
	}

	/*add the arguments of the command and add if the command exist twise*/
	instructionLength += arguments->length;
	instructionLength = instructionLength* returnTimes;
	
	if (arguments->length == 2) {
		destination = getValueAsInt(arguments->tail->value);
		source = getValueAsInt(arguments->head->value);
		
		if (DIRECT_REGISTER_ADDRESSING_VALUE == source && DIRECT_REGISTER_ADDRESSING_VALUE == destination)
			instructionLength -= 1 * returnTimes;
			
	}
	addNode(holder->instructions, NULL, createInstruction(command, arguments));
	holder->counter += instructionLength;
}

Command* searchCommand(NodesList* commands, char* name)
{
	/*
	Search command and relocate memory for him cause of change the ReturnTimes in Instruction
	*/
	Command* toReturn = safeMalloc(sizeof(Command));
	Node* currentNode = commands->head;

	while (currentNode)
	{
		if (startsWith(name, currentNode->name))
		{
			memcpy(toReturn, currentNode->value, sizeof(Command));
			return toReturn;
		}	
		currentNode = currentNode->next;
	}
	return NULL;
}

Instruction* createInstruction(Command* command, NodesList* arguments) {
	Instruction* instruction = (Instruction*)safeMalloc(sizeof(Instruction));
	instruction->command = command;
	instruction->arguments = arguments;
	return instruction;
}

bool extractCommandParams(NodesList* arguments, AssemblyLine* assemblyLine, InstructionsHolder* holder, char** lastArgument) {
	const char* delimiters = ",";
	char* token;
	char* argument;
	int addressingType;
	

	/* walk through the tokens */
	for (token = strtok(assemblyLine->line, delimiters); token != NULL; token = strtok(NULL, delimiters))
	{
		token = trim(token);
		if (token[0] == LAST_ADDRESSING_START && token[1] == LAST_ADDRESSING_START) { /* It is a last argument */
			if (NULL != *lastArgument)
			{
				argument = *lastArgument;
			}
			else
			{
				addError(holder->errorHolder, "this command request the last src arguments but he doesnt exist", assemblyLine->lineNumber);
				return FALSE;
			}
		}
		else
		{
			argument = trim(token);
		}
		addressingType = getAddressingType(assemblyLine, argument, holder);
		addNode(arguments, argument, &addressingType);
	}
	return TRUE;
}

int extractCommandReturnTimes(char* assemblyLine)
{
	/*
	extract the return times that the command exist (movX)
	*/
	int length;
	int i;

	length = strlen(assemblyLine);

	for (i = 0; i < length; i++)
	{
		if (isdigit(assemblyLine[i]))
		{
			return assemblyLine[i] - '0';
		}
	}

	return 0;
}
int getAddressingType(AssemblyLine* assemblyLine, char* argument, InstructionsHolder* holder) {
	if (argument[0] == IMMEDIATE_ADDRESSING_START)
		return IMMEDIATE_ADDRESSING_VALUE;
	
	else if (argument[0] == LAST_ADDRESSING_START)
		return LAST_ADDRESSING_VALUE;
	
	else if (searchNode(holder->registers, argument))
		return DIRECT_REGISTER_ADDRESSING_VALUE;
	
	else {
		addNode(holder->symbolsUsedInArguments, argument, &(assemblyLine->lineNumber));
		return DIRECT_ADDRESSING_VALUE;
	}
}

void addInstructionSymbol(InstructionsHolder* holder, AssemblyLine* assemblyLine) {
	if (assemblyLine->labelName) {
		if (searchNode(holder->symbols, assemblyLine->labelName))
			addError(holder->errorHolder, "This label was already declared", assemblyLine->lineNumber);
		
		else
			addNode(holder->symbols, assemblyLine->labelName, &holder->counter);
	}
}

char* getCommandName(char** linePtr) {
	const char commandDelimiter = ' ';
	return seperateString(linePtr, commandDelimiter);
}

InstructionsHolder* CreateInstructions() {
	InstructionsHolder* holder = (InstructionsHolder*)safeMalloc(sizeof(InstructionsHolder));
	
	/*init new lists*/
	holder->commands = createNodesList(sizeof(Command));
	holder->symbols = createNodesList(sizeof(int));
	holder->symbolsUsedInArguments = createNodesList(sizeof(int));
	holder->instructions = createNodesList(sizeof(Instruction));
	holder->registers = createNodesList(sizeof(char*));
	
	/*add relevant values*/
	holder->counter = MEMORY_ADDRESS_START;
	addRegisters(holder->registers);
	addCommands(holder->commands);
	holder->errorHolder = createErrorHolder();
	
	return holder;
}

void freeInstructions(InstructionsHolder* holder) {
	freeNodesList(holder->symbols);
	freeNodesList(holder->commands);
	freeNodesList(holder->registers);
	freeNodesList(holder->symbolsUsedInArguments);
	freeInstructionsList(holder->instructions);
	freeErrorHolder(holder->errorHolder);
	free(holder);
}

void freeInstructionsList(NodesList* instructions) {
	Node* node;
	Instruction* instruction;
	while ( (node = getNext(instructions)) ) {
		instruction = (Instruction*)(node->value);
		freeNodesList(instruction->arguments);
	}
	freeNodesList(instructions);
}

void addRegisters(NodesList* registers) {
	addRegister(registers, REG0, REG0_VALUE);
	addRegister(registers, REG1, REG1_VALUE);
	addRegister(registers, REG2, REG2_VALUE);
	addRegister(registers, REG3, REG3_VALUE);
	addRegister(registers, REG4, REG4_VALUE);
	addRegister(registers, REG5, REG5_VALUE);
	addRegister(registers, REG6, REG6_VALUE);
	addRegister(registers, REG7, REG7_VALUE);
}

void addRegister(NodesList* registers, char* registerName, int registerValue)
{
	addNode(registers, registerName, &registerValue);
}

void addCommands(NodesList* commands) {
	addCommand(commands, MOV_STR, MOV_CODE, MOV_ARGS, checkMovArgs);
	addCommand(commands, CMP_STR, CMP_CODE, CMP_ARGS, checkCmpArgs);
	addCommand(commands, ADD_STR, ADD_CODE, ADD_ARGS, checkAddArgs);
	addCommand(commands, SUB_STR, SUB_CODE, SUB_ARGS, checkSubArgs);
	addCommand(commands, LEA_STR, LEA_CODE, LEA_ARGS, checkLeaArgs);

	addCommand(commands, NOT_STR, NOT_CODE, NOT_ARGS, checkNotArgs);
	addCommand(commands, CLR_STR, CLR_CODE, CLR_ARGS, checkClrArgs);
	addCommand(commands, INC_STR, INC_CODE, INC_ARGS, checkIncArgs);
	addCommand(commands, DEC_STR, DEC_CODE, DEC_ARGS, checkDecArgs);
	addCommand(commands, JMP_STR, JMP_CODE, JMP_ARGS, checkJmpArgs);
	addCommand(commands, BNE_STR, BNE_CODE, BNE_ARGS, checkBneArgs);
	addCommand(commands, RED_STR, RED_CODE, RED_ARGS, checkRedArgs);
	addCommand(commands, PRN_STR, PRN_CODE, PRN_ARGS, checkPrnArgs);
	addCommand(commands, JSR_STR, JSR_CODE, JSR_ARGS, checkJsrArgs);

	addCommand(commands, RTS_STR, RTS_CODE, RTS_ARGS, checkRtsArgs);
	addCommand(commands, STOP_STR, STOP_CODE, STOP_ARGS, checkStopArgs);
}

void addCommand(NodesList* commands, char* commandName, int code, int numberOfArguments, bool(*correctArgumentsTypes)(NodesList*)) {
	Command* command = createCommand(code, numberOfArguments, correctArgumentsTypes);
	addNode(commands, commandName, command);
	free(command); /* When adding a node, it copies the bytes, so we can free this command */
}

Command* createCommand(int code, int numberOfArguments, bool(*correctArgumentsTypes)(NodesList*)) {
	Command* command = (Command*)safeMalloc(sizeof(Command));
	command->code = code;
	command->numberOfArguments = numberOfArguments;
	command->numOfReturnTimes = 0;
	command->correctArgumentsTypes = correctArgumentsTypes;
	return command;
}
