#include "General.h"
#include "NodesList.h"
#include "InstructionHandler.h"
#include "GuidelineHandler.h"
#include "Word.h"

NodesList* translateInforamtion(InstructionsHolder* instructionsHolder, GuidelineHolder* guidelineHolder, NodesList* dataSymbols, NodesList* externSymbols);

void translateInstructions(int* wordAddress, NodesList* words, InstructionsHolder* holder, NodesList* dataSymbols, NodesList* externSymbols);

void translateData(int* wordAddress, NodesList* words, GuidelineHolder* holder, int counterToAdd);

FirstWord* createFirstWord(int destination, int source, int opcode, int group);

ArgumentWord* argumentToWord(int instructionAddress, int wordAddress, Node* argumentNode, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols, NodesList* registers);

void translateDistanceToArgument(char* argument, char** label1, char** label2);

int distanceArgumentAddress(char* label, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols);

int getMaxBetweenThree(int a, int b, int c);

WordHolder* createWordHolder(int address, int* wordValue);

void addFirstWord(int* wordAddress, NodesList* words, Instruction* instruction);

void addArgumentsWords(int instructionAddress, int* wordAddress, NodesList* words, NodesList* arguments, NodesList* instructionSymbols, NodesList* dataSymbols, NodesList* externSymbols, NodesList* registers);