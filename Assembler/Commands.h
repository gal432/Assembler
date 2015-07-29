#ifndef COMMANDS_H
#define COMMANDS_H

#include "General.h"
#include "NodesList.h"

#define IMMEDIATE_ADDRESSING_START '#'
#define IMMEDIATE_ADDRESSING_VALUE 0

#define DIRECT_ADDRESSING_VALUE 1

#define LAST_ADDRESSING_START '$'
#define LAST_ADDRESSING_VALUE 2

#define DIRECT_REGISTER_ADDRESSING_VALUE 3

/* 2 arguments commands */
#define MOV_STR "mov"
#define MOV_CODE 0
#define MOV_ARGS 2

#define CMP_STR "cmp"
#define CMP_CODE 1
#define CMP_ARGS 2

#define ADD_STR "add"
#define ADD_CODE 2
#define ADD_ARGS 2

#define SUB_STR "sub"
#define SUB_CODE 3
#define SUB_ARGS 2

#define LEA_STR "lea"
#define LEA_CODE 6
#define LEA_ARGS 2

/* 1 arguments commands */
#define NOT_STR "not"
#define NOT_CODE 4
#define NOT_ARGS 1

#define CLR_STR "clr"
#define CLR_CODE 5
#define CLR_ARGS 1

#define INC_STR "inc"
#define INC_CODE 7
#define INC_ARGS 1

#define DEC_STR "dec"
#define DEC_CODE 8
#define DEC_ARGS 1

#define JMP_STR "jmp"
#define JMP_CODE 9
#define JMP_ARGS 1

#define BNE_STR "bne"
#define BNE_CODE 10
#define BNE_ARGS 1

#define RED_STR "red"
#define RED_CODE 11
#define RED_ARGS 1

#define PRN_STR "prn"
#define PRN_CODE 12
#define PRN_ARGS 1

#define JSR_STR "jsr"
#define JSR_CODE 13
#define JSR_ARGS 1

/* 0 arguments commands */

#define RTS_STR "rts"
#define RTS_CODE 14
#define RTS_ARGS 0

#define STOP_STR "stop"
#define STOP_CODE 15
#define STOP_ARGS 0

/* For each command we have a function that test if the types of the arguments are good */

bool checkMovArgs(NodesList* arguments);
bool checkCmpArgs(NodesList* arguments);
bool checkAddArgs(NodesList* arguments);
bool checkSubArgs(NodesList* arguments);
bool checkNotArgs(NodesList* arguments);
bool checkClrArgs(NodesList* arguments);
bool checkLeaArgs(NodesList* arguments);
bool checkIncArgs(NodesList* arguments);
bool checkDecArgs(NodesList* arguments);
bool checkJmpArgs(NodesList* arguments);
bool checkBneArgs(NodesList* arguments);
bool checkRedArgs(NodesList* arguments);
bool checkPrnArgs(NodesList* arguments);
bool checkJsrArgs(NodesList* arguments);
bool checkRtsArgs(NodesList* arguments);
bool checkStopArgs(NodesList* arguments);


#endif