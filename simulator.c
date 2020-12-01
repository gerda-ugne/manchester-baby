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