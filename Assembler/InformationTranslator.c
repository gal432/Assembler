#include <string.h>
#include <math.h>
#include "InformationTranslator.h"
#include "StringFuncs.h"

NodesList* translateInforamtion(InstructionsHolder* instructionsHolder, GuidelineHolder* guidelineHolder, NodesList* dataSymbols, NodesList* externSymbols) {
	NodesList* words = createNodesList(sizeof(WordHolder));
	int wordAddress = MEMORY_ADDRESS_START;
	translateInstructions(&wordAddress, words, instructionsHolder, dataSymbols, externSymbols);
	translateData(&wordAddress, words, guidelineHolder, instructionsHolder->counter);
	return words;
}

void translateInstructions(int* wordAddress, NodesList* words, InstructionsHolder* holder, NodesList* dataSymbols, NodesList* externSymbols)
{
	Node* instructionNode;
	Instruction* instruction;
	int instructionAddress;
	while ((instructionNode = (getNext(holder->instructions))))
	{
		instruction = (Instruction*)(instructionNode->value);
		instructionAddress = *wordAddress;
		addFirstWord(wordAddress, words, instruction);
		addArgumentsWords(instructionAddress, wordAddress, words, instruction->arguments, holder->symbols, dataSymbols, externSymbols, holder->registers);
	}
}

void translateData(int* wordAddress, NodesList* words, GuidelineHolder* holder, int counterToAdd)
{
	int i;
	DataWord* word;
	WordHolder* wordHolder = NULL;
	for (i = 0; i < holder->counter; i++)
	{
		word = (DataWord*)safeMalloc(sizeof(DataWord));
		word->value = holder->memory[i];
		wordHolder = createWordHolder(i + counterToAdd, (int*)word);
		addNode(words, NULL, wordHolder);
		free(wordHolder);
		free(word);
	}
}

void addFirstWord(int* wordAddress, NodesList* words, Instruction* instruction)
{
	FirstWord* firstWord;
	WordHolder* wordHolder;
	int destination = 0, source = 0;
	if (instruction->arguments->length >= 1) {
		destination = *((int*)(instruction->arguments->tail->value));
	}

	if (instruction->arguments->length == 2) {
		source = *((int*)(instruction->arguments->head->value));
	}
	firstWord = createFirstWord(destination, source, instruction->command->code, instruction->arguments->length);
	wordHolder = createWordHolder(*wordAddress, (int*)firstWord);
	(*wordAddress)++;
	addNode(words, NULL, wordHolder);
	free(wordHolder);
	free(firstWord);
}

void addArgumentsWords(int instructionAddress, int* wordAddress, NodesList* words, NodesList* arguments, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols, NodesList* registers)
{
	Node* argumentNode;
	ArgumentWord* word;
	RegisterArgumentWord* registerWord;
	Node* registerNode;
	WordHolder* wordHolder;
	if (arguments->length == 2 && getValueAsInt(arguments->head->value) == DIRECT_REGISTER_ADDRESSING_VALUE && getValueAsInt(arguments->tail->value) == DIRECT_REGISTER_ADDRESSING_VALUE){
		registerWord = (RegisterArgumentWord*)safeMalloc(sizeof(RegisterArgumentWord));
		registerNode = searchNode(registers, arguments->head->name);
		registerWord->left_value = getValueAsInt(registerNode->value);
		registerNode = searchNode(registers, arguments->tail->name);
		registerWord->right_value = getValueAsInt(registerNode->value);
		registerWord->encodingType = ABSOLUTE_ENCODING;
		wordHolder = createWordHolder(*wordAddress, (int*)registerWord);
		(*wordAddress)++;
		addNode(words, NULL, wordHolder);
		free(wordHolder);
		free(registerWord);
	}
	else
	{
		while ((argumentNode = getNext(arguments))) {
			word = argumentToWord(instructionAddress, *wordAddress, argumentNode, instructionSymbols, dataSymbols, externSymbols, registers);
			wordHolder = createWordHolder(*wordAddress, (int*)word);
			(*wordAddress)++;
			addNode(words, NULL, wordHolder);
			free(wordHolder);
			free(word);
		}
	}
}

FirstWord* createFirstWord(int destination, int source, int opcode, int group) {
	FirstWord* word = (FirstWord*)safeMalloc(sizeof(FirstWord));
	word->encodingType = ABSOLUTE_ENCODING;
	word->destination = destination;
	word->source = source;
	word->opcode = opcode;
	word->group = group;
	return word;
}

ArgumentWord* argumentToWord(int instructionAddress, int wordAddress, Node* argumentNode, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols, NodesList* registers) {
	ArgumentWord* word;
	RegisterArgumentWord* registerWord;
	Node* symbol;
	int addressingType;
	char* parser;
	char* label1, *label2;
	int label1Value, label2Value;
	word = NULL;
	registerWord = NULL;
	addressingType = getValueAsInt(argumentNode->value);
	if (addressingType == IMMEDIATE_ADDRESSING_VALUE){
		word = (ArgumentWord*)safeMalloc(sizeof(ArgumentWord));
		parser = argumentNode->name + 1; /* Skip the first char that represents that it's an IMMEDIATE_ADDRESSING */
		word->value = atoi(parser);
		word->encodingType = ABSOLUTE_ENCODING;
	}
	else if (addressingType == DIRECT_ADDRESSING_VALUE) {
		word = (ArgumentWord*)safeMalloc(sizeof(ArgumentWord));
		if ((symbol = searchNode(instructionSymbols, argumentNode->name))) {
			word->value = getValueAsInt(symbol->value);
			word->encodingType = ABSOLUTE_ENCODING;
		}
		else if((symbol = searchNode(dataSymbols, argumentNode->name))) {
			word->value = getValueAsInt(symbol->value);
			word->encodingType = RELOCATABLE_ENCODING;
		}
		else if ((symbol = searchNode(externSymbols, argumentNode->name))) {
			word->value = 0;
			word->encodingType = EXTERNAL_ENCODING;
			addNode((NodesList*)(symbol->value), NULL, &wordAddress);
		}
	}
	else if (addressingType == DISTANCE_ADDRESSING_VALUE){
		word = (ArgumentWord*)safeMalloc(sizeof(ArgumentWord));
		translateDistanceToArgument(argumentNode->name, &label1, &label2);
		label1Value = distanceArgumentAddress(label1, instructionSymbols, dataSymbols, externSymbols);
		label2Value = distanceArgumentAddress(label2, instructionSymbols, dataSymbols, externSymbols);
		word->value = getMaxBetweenThree(label1Value - label2Value, label1Value - instructionAddress, label2Value - instructionAddress);
		word->encodingType = ABSOLUTE_ENCODING;
	}
	else if (addressingType == DIRECT_REGISTER_ADDRESSING_VALUE){
		registerWord = (RegisterArgumentWord*)safeMalloc(sizeof(RegisterArgumentWord));
		if ((symbol = searchNode(registers, argumentNode->name))) {
			int value = getValueAsInt(symbol->value);
			if (wordAddress - instructionAddress == 1)
			{
				/* This means this is the destination argument */
				registerWord->left_value = value;
				registerWord->right_value = 0;
			}
			else
			{
				registerWord->left_value = 0;
				registerWord->right_value = value;
			}
			registerWord->encodingType = ABSOLUTE_ENCODING;
		}
	}
	if (word)
		return word;
	return (ArgumentWord*)registerWord; /* They are the same size, so the inteager they represent will be the same */
}

void translateDistanceToArgument(char* argument, char** label1, char** label2)
{
	argument += 2; /* Skip the first char that represents that it's an DISTANCE_ADDRESSING_VALUE and the second char('(') */
	*(argument + strlen(argument) - 1) = '\0'; /* Cut the last char (')') */
	*label1 = strtok(argument, ",");
	*label2 = strtok(NULL, ",");
	*label1 = trim(*label1);
	*label2 = trim(*label2);
}

int distanceArgumentAddress(char* label, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols)
{
	Node* symbol;
	if ((symbol = searchNode(instructionSymbols, label))) {
		return getValueAsInt(symbol->value);
	}
	else if ((symbol = searchNode(dataSymbols, label))) {
		return getValueAsInt(symbol->value);
	}
	else if ((symbol = searchNode(externSymbols, label))) {
		return 0;
	}
	return -1;
}

int getMaxBetweenThree(int a, int b, int c)
{
	int temp;
	a = abs(a);
	b = abs(b);
	c = abs(c);
	temp = a > b ? a : b;
	return temp > c ? temp : c;
}

WordHolder* createWordHolder(int address, int* wordValue) {
	WordHolder* wordHolder = (WordHolder*)safeMalloc(sizeof(WordHolder));
	wordHolder->address = address;
	wordHolder->value = *wordValue & BITS_TO_ZERO_START;
	return wordHolder;
}