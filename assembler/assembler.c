#include <stdio.h>
#include "assembler.h"


typedef struct instructionSet{
char stringInstruction[10];
char binaryInstruction[8];

} InstructionSet;

/**/
InstructionSet instructions[8];
int codeBuffer[8];
/*Stores memory adresses of user's labels and variables*/
int symbolTable[8];
/*Loads and runs object code*/
int loader;

void setInstructions()
{

}