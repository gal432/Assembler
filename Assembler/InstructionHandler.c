#include "InstructionHandler.h"

void handleInstruction(InstructionsHolder* holder, AssemblyLine* assemblyLine) {
	int instructionLength;
	int destination;
	int source;
	int zeroArguments;
	char* commandName;
	
	Node* node;
	NodesList* arguments;
	Command* command;
	
	zeroArguments = FALSE;
	source = 0;
	destination = 0;
	instructionLength = 1;

	addInstructionSymbol(holder, assemblyLine);
	commandName = getCommandName(&(assemblyLine->line));
	
	/* Command without arguments */
	if (!commandName) {
		commandName = assemblyLine->line; 
		zeroArguments = TRUE;
	}
	node = searchNode(holder->commands, commandName);	

	if (NULL == node) {
		addError(holder->errorHolder, "Command name is invalid", assemblyLine->lineNumber);
		return;
	}

	arguments = createNodesList(sizeof(int));
	command = (Command*)node->value;
	
	if (!zeroArguments)
		extractCommandParams(arguments, assemblyLine, holder);

	if (arguments->length != command->numberOfArguments) {
		addError(holder->errorHolder, "Number of command arguments was wrong", assemblyLine->lineNumber);
		return;
	}

	if (!command->correctArgumentsTypes(arguments)) {
		addError(holder->errorHolder, "Wrong type of arguments", assemblyLine->lineNumber);
		return;
	}
	instructionLength += arguments->length;
	if (arguments->length == 2) {
		destination = getValueAsInt(arguments->tail->value);
		source = getValueAsInt(arguments->head->value);
		if (DIRECT_REGISTER_ADDRESSING_VALUE == source && DIRECT_REGISTER_ADDRESSING_VALUE == destination)
			instructionLength--;
	}
	addNode(holder->instructions, NULL, createInstruction(command, arguments));
	holder->counter += instructionLength;
}

Instruction* createInstruction(Command* command, NodesList* arguments) {
	Instruction* instruction = (Instruction*)safeMalloc(sizeof(Instruction));
	instruction->command = command;
	instruction->arguments = arguments;
	return instruction;
}

NodesList* extractCommandParams(NodesList* arguments, AssemblyLine* assemblyLine, InstructionsHolder* holder) {
	const char* delimiters = ",";
	char* token;
	char* argument;
	int addressingType;	

	/* walk through the tokens */
	for (token = strtok(assemblyLine->line, delimiters); token != NULL; token = strtok(NULL, delimiters))
	{
		token = trim(token);
		if (token[0] == DISTANCE_ADDRESSING_START) { /* It is a distance argument */
			strtok(NULL, delimiters); /* Skip the next one */
			*(token + strlen(token)) = ','; /* Make the end of the token to be back to what it was */
		}
		argument = trim(token);
		addressingType = getAddressingType(assemblyLine, argument, holder);
		addNode(arguments, argument, &addressingType);
	}
	return arguments;
}

int getAddressingType(AssemblyLine* assemblyLine, char* argument, InstructionsHolder* holder) {
	if (argument[0] == IMMEDIATE_ADDRESSING_START)
		return IMMEDIATE_ADDRESSING_VALUE;
	
	else if (argument[0] == DISTANCE_ADDRESSING_START)
		return DISTANCE_ADDRESSING_VALUE;
	
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
	command->correctArgumentsTypes = correctArgumentsTypes;
	return command;
}