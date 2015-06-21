#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "General.h"
#include "NodesList.h"
#include "AssemblyCompiler.h"
#include "StringFuncs.h"
#include "AssemblyLine.h"

/* Reads the lines from the file */
NodesList* readLinesToAssemblyList(char* fileName);

/* frees the strings of the lines */
void freeLinesString(NodesList* lines); 
#endif