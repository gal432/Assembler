#include "Commands.h"

bool checkMovArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || LAST_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkCmpArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || LAST_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(IMMEDIATE_ADDRESSING_VALUE == destinationArgument || DIRECT_ADDRESSING_VALUE == destinationArgument || LAST_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkAddArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || LAST_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkSubArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == sourceArgument || DIRECT_ADDRESSING_VALUE == sourceArgument || LAST_ADDRESSING_VALUE == sourceArgument || DIRECT_REGISTER_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkNotArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkClrArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkLeaArgs(NodesList* arguments) {
	int sourceArgument = *((int*)(arguments->head->value));
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == sourceArgument)
		&&
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkIncArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkDecArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkJmpArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || LAST_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkBneArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || LAST_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkRedArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(DIRECT_ADDRESSING_VALUE == destinationArgument || LAST_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkPrnArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == destinationArgument || DIRECT_ADDRESSING_VALUE == destinationArgument || LAST_ADDRESSING_VALUE == destinationArgument || DIRECT_REGISTER_ADDRESSING_VALUE == destinationArgument);
}

bool checkJsrArgs(NodesList* arguments) {
	int destinationArgument = *((int*)(arguments->tail->value));
	return
		(IMMEDIATE_ADDRESSING_VALUE == destinationArgument);
}

bool checkRtsArgs(NodesList* arguments) {
	return TRUE;
}

bool checkStopArgs(NodesList* arguments) {
	return TRUE;
}
