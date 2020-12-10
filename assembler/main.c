#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "assembler.h"

int main()
{
	//ALlocate memory for buffer and symbol table
	createBuffer();
	createSymbolTable();
	//Initalize the instruction set
	initialiseInstructionSet();


	char readLines[256][256];
	//Load the code into the assembler
	loadCode(readLines);

	//Complete the first pass of the assembler
	firstPass(readLines);

	//Write the outputs to a file
	writeToFile("text.txt",32);
	//Clear the memory
	clearSymbolTable();
	clearBuffer();
    return 0;
}