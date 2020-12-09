#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "assembler.h"

int main()
{

	createBuffer();
	createSymbolTable();

	char readLines[256][256];

	loadCode(readLines);

	initialiseInstructionSet();

	firstPass(readLines);
	
    return 0;
}