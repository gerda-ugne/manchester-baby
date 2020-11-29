#ifndef _SIMULATOR_
#define _SIMULATOR_

/*ERROR CODES*/

#define SUCCESS                  100
#define INVALID_INPUT_PARAMETER  101
#define MEMORY_ALLOCATION_ERROR  102   
#define INVALID_LIST_OPERATION   103
#define FILE_IO_ERROR            104

/*METHODS OF THE SIMULATOR*/

/*Runs the Manchester Baby simulator*/
void runSimulator();
/*Increments the control instruction*/
void incrementCI();
/*Fetches instructions from the store*/
void fetch();
/*Decodes the fetch instruction*/
void decode();
/*Displays all relevant data*/
void display();

#endif