#include <stdlib.h>
#include <string.h> 
#include <stdio.h>
#include "simulator.h"

/*Holds the default instruction set of the machine*/
static const int instructionSet[8];
/*Holds the adress of the current line in store for the next instruction fetch*/
static int controlInstruction = 0;
/*Defines the memory size of the machine*/
static int bits = 32;
/*Contains the machine memory of size bits x bits*/
static int** store;
/*Containts the register of the machine, size of bits*/
static int* accumulator;
/*Holds the opcode of the instruction to execute*/
static int* presentInstruction;
/*Indicates which line of the store the present intruction should operate on*/
static int lineNumber;


int main()
{
    //Void to avoid compilator complaints
    (void)instructionSet;

    allocateMemory();
    runSimulator(store);
    return 0;
}

/**
 * Runs the Manchester Baby simulator
 * */
void runSimulator()
{
    incrementCI();
    fetch();
}

/**
 * Increments the control instruction by 1.
 * */
void incrementCI()
{
    controlInstruction++;
}

/**
 * Allocates memory for the accumulator and the present instruction.
 * @return SUCCESS or MEMORY_ALLOCATION_ERROR if malloc was unsuccesful.
*/
int allocateMemory()
{
    accumulator = (int*)malloc(sizeof(int));
    presentInstruction = (int*)malloc(sizeof(int));
    initialiseStore();
    
    return (accumulator==NULL || presentInstruction==NULL || store==NULL ) ? MEMORY_ALLOCATION_ERROR: SUCCESS;
}

/**
 * Frees the memory that has been allocated.
 * */
void freeMemory()
{
    free(accumulator);
    free(presentInstruction);

    //To implement
    freeStore();

}

int initialiseStore()
{
    return 1;
}

void freeStore()
{

}

/**
 * Changes the bit value for the program. 
 * Default is 32.
 * 
 * @param newValue - new value to be assigned
 * */
void changeBits (int newValue)
{
    bits = newValue;
}

/**
 * Fetches a new line directed by the control instruction
 * from the store.
 
 * @return SUCCESS if valid, INVALID_FETCH if there is nothing to fetch, INVALID_PARAMETER if space was not allocated
 */
int fetch()
{   
    //Check if variables had space allocated
    if(accumulator==NULL) return INVALID_PARAMETER;
    if(store == NULL) return INVALID_PARAMETER;

    //If there is nothing to fetch return error
    if(store[controlInstruction]==NULL) return INVALID_FETCH;
    memcpy(accumulator, store[controlInstruction], bits*sizeof(int));


    return SUCCESS;

}

/*Decodes the fetch instruction*/
void decode()
{
	lineNumber=accumulator[0]+accumulator[1]*2+accumulator[2]*4+accumulator[3]*8+accumulator[4]*16;
	*presentInstruction=accumulator[13]+accumulator[14]*2+accumulator[15]*4;
}

/*Converts a binary number to an integer.
 *@param binaryArray - a pointer to an array of numbers the length of bits (32 by default)
 *@return the converted number
*/
int convertBinaryToInt(int *binaryArray)
{
	int number = 0;
	for(int i=0; i<bits; i++)
	{
		number+=(binaryArray[i]*raiseToPower(2,i));
	}
	return number;
}

/* Raises a number to a power
 *@param number - the base number
 *@param power - the power to which the number has to be raised
 *@return the number raised to the power
*/
int raiseToPower(int number, int power)
{
	int result = 1;
	for(int i=0; i<power; i++)
	{
		result*=number;
	}
	return result;
}

/*Executes the decoded instruction, returns -1 if program has ended, or SUCCESS if instruction successfully executed*/
int execute()
{
	switch(*presentInstruction) {
		// Jump to the instruction at the address obtained from the specified memory address at lineNumber
		case 0: //JMP
			controlInstruction=*store[lineNumber];
			return SUCCESS;
		// Jump to the instruction at controlInstruction plus lineNumber
		case 1: //JRP
			controlInstruction+=lineNumber;
			return SUCCESS;
		// Load the number from store[lineNumber] to accumulator
		case 2: //LDN
			memcpy(accumulator, store[lineNumber], bits*sizeof(int));
			return SUCCESS;
		// Stores number in accumulator to store[lineNumber]
		case 3: //STO
			memcpy(store[lineNumber], accumulator, bits*sizeof(int));
			return SUCCESS;
		// Subtracts number at store[lineNumber] from accumulator
		case 4: //SUB
			accumulator-=convertBinaryToInt(store[lineNumber]);
			return SUCCESS;
		//Does the same as case 4
		case 5: //SUB
			accumulator-=convertBinaryToInt(store[lineNumber]);
			return SUCCESS;
		// If accumulator contains a negative value, skip next instruction
		case 6: //CMP
			if(convertBinaryToInt(accumulator)<0) controlInstruction++;
			return SUCCESS;
		// Stop program
		case 7: //STP
			return -1;
	}
}
