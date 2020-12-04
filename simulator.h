#ifndef _SIMULATOR_
#define _SIMULATOR_
/*ERROR CODES*/
#define SUCCESS                  100
#define INVALID_PARAMETER        101
#define MEMORY_ALLOCATION_ERROR  102   
#define INVALID_OPERATION        103
#define FILE_IO_ERROR            104
#define INVALID_FETCH            105
#define OUT_OF_BOUNDARIES        106
#define INVALID_MACHINE_CODE     107

/*METHODS OF THE SIMULATOR*/
/* Increments the control instruction by 1.*/
void incrementCI();
/* Allocates memory for the accumulator and the present instruction.*/
int allocateMemory();
/*Frees the memory that has been allocated.*/
void freeMemory();
/*Initializes the values for the store*/
void changeBits (int newValue);
/*Runs the Manchester Baby simulator*/
void runSimulator();
/*Increments the control instruction*/
void incrementCI();
/*Fetches instructions from the store*/
int fetch();
/*Decodes the fetch instruction*/
void decode();
/*Transforms backwards binary code to integer*/
int convertBinaryToInt(int *binaryArray);
/*Raises number to a power*/
int raiseToPower(int number, int power);
/*Executes the instruction, returns -1 if program has ended*/
int execute();
/*Displays all relevant data*/
void display();
/*Loads the machine code from the Assembler generated file*/
void loadCode();

/*Initialises Store*/
int initialiseStore();
/*Fills Store with given program code*/
int fillStore(char fileName[]);
/*Displays Store*/
void displayStore();
/*Deletes Store*/
int freeStore();
/*Resets Store to its default values*/
void resetStore(int row);
/*Displays the contents of the accumulator*/
void displayAccumulator();
/*Displays the contents of the present instruction*/
void displayPresentInstruction();

#endif
