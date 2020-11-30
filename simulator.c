#include <stdio.h>
#include "simulator.h"

/*Holds the default instruction set of the machine*/
static const instructionSet[8];
/*Holds the adress of the current line in store for the next instruction fetch*/
static int controlInstruction = 0;
/*Defines the memory size of the machine*/
static int bits = 32;


/*Containts the machine memory of size bits x bits*/
int store[][];
/*Containts the register of the machine, size of bits*/
int accumulator[];
/*Holds the opcode of the instruction to execute*/
int presentInstruction[];


int main()
{
    initializeValues();
    runSimulator();
    return 0;
}
