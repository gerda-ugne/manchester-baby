#ifndef _SIMULATOR_
#define _SIMULATOR_

/*ERROR CODES*/

#define SUCCESS                  100
#define INVALID_PARAMETER  101
#define MEMORY_ALLOCATION_ERROR  102   
#define INVALID_OPERATION        103
#define FILE_IO_ERROR            104
#define OUT_OF_BOUNDARIES        105
#define INVALID_FETCH            106

/*METHODS OF THE SIMULATOR*/


/* Increments the control instruction by 1.*/
void incrementCI();
/* Allocates memory for the accumulator and the present instruction.*/
int allocateMemory();
/*Frees the memory that has been allocated.*/
void freeMemory();
/*Initializes the values for the store*/
int initialiseStore();
/*Frees the allocated memory of the store*/
void freeStore();
/*Changes the bit value for the program. */
void changeBits (int newValue);
/*Runs the Manchester Baby simulator*/
void runSimulator();
/*Increments the control instruction*/
void incrementCI();
/*Fetches instructions from the store*/
int fetch();
/*Decodes the fetch instruction*/
void decode();
/*Displays all relevant data*/
void display();
/*Loads the machine code from the Assembler generated file*/
void loadCode();

#endif