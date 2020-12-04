#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "simulator.h"

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
/*Holds the fetched instruction line*/
static int* fetchedInstruction;

/**
 * Runs the Manchester Baby simulator
 * */
void runSimulator()
{
    int fetchStatus = 0;
    int executeStatus = 0;
    //While there are lines to fetch and the loop is not halted
    while(fetchStatus!=INVALID_FETCH && executeStatus != -1)
    {
        incrementCI();
        fetchStatus = fetch();
        decode();
        executeStatus = execute();
        displayAccumulator();

    }
    
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
    accumulator = (int*)calloc(bits, sizeof(int));
    presentInstruction = (int*)calloc(bits, sizeof(int));
    fetchedInstruction = (int*)calloc(bits, sizeof(int));
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
    free(fetchedInstruction);
    freeStore();

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
    if(fetchedInstruction == NULL) return INVALID_PARAMETER;
    if(store == NULL) return INVALID_PARAMETER;

    //If there is nothing to fetch return error
    if(store[controlInstruction]==NULL) 
    {
        //Reset the accumulator
        free(fetchedInstruction);
        fetchedInstruction = (int*)calloc(bits, sizeof(int));
        return INVALID_FETCH;
    }
    else
    {
        memcpy(fetchedInstruction, store[controlInstruction], bits*sizeof(int));
    }


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

    return -1;
}
  
/*Initialises  Store to its default values 0 
@return SUCCESS if initialization was successful otherwise MEMORY_ALLOCATION_ERROR was detected
*/
int initialiseStore()
{
    store = (int**)malloc(sizeof(int*)*bits);
    if(store == NULL) return MEMORY_ALLOCATION_ERROR;

    
    for(int i =0; i<bits; i++)
    {
        store[i]=(int*)calloc(bits, sizeof(int)*bits);
        if(store[i] == NULL) return MEMORY_ALLOCATION_ERROR;
    }

    return SUCCESS;
}
/*Reads  a machine code from a text file and stores it in a Store
@return SUCCESS: machine code was successfully stored in memory
        INVALID_PARAMETER : file name was not found in the current directory
        FILE_IO_ERROR : file was opened/closed unsuccessfully
        INVALID_MACHINE_CODE : machine code contains invalid characters or is capacity is not compatibale with memory
*/
int fillStore(char fileName[] )
{
    if(fileName == NULL || strlen(fileName) > 50 || strlen(fileName) == 0)
        return INVALID_PARAMETER;
    FILE *f;
    f = fopen(fileName, "r");
    if(f==NULL)
        return FILE_IO_ERROR; 
        
    int counter=0;
    char line[256]; 
    //While there is a new line and the number of lines in memory is not exceeded
    while((fgets(line, 256,f) != NULL) && counter < bits)    
    {
        int len =strlen(line);

        //Validation: checking if a program is not 64bits when the memory can hold only 32bits
        if(bits == 32 && len > 60)
        { 
            resetStore(counter);
            return INVALID_MACHINE_CODE;
        }
        
        //Making sure that if memory is of 64 bits, a program of 32 bits could be stored 
        int index = bits;
        if(len < 40 && bits == 64) 
            index =32;
        else if(bits > 40)
            index =64;

        //Creating a duplicate array that would contain only 32 or 64 elements, since a line of characters
        //that is read in contains some extra chars that are not needed (such as \n) 
        char array[index];
        for(int i =0; i<index; i++)
        {
            array[i] = line[i];
        }
    
        for(int i =0; i<index; i++)
        {
            int number=array[i] - '0' ;  // Converting from char into int
            if(number == 0 || number == 1)
                store[counter][i] = number;
            else
            {
                //Validation: if any invalid characters(like a letter or a number that is neither 1 or 0) are detected
                //the store is reset to its default values 
                resetStore(counter);
                return INVALID_MACHINE_CODE;
            }
        }
        counter++;
    }

    if(fclose(f) != 0)
        return FILE_IO_ERROR;
    return SUCCESS;
}

/*Displays current state of a memory - Store*/
void displayStore()
{
    for(int i =0; i<bits; i++)
    {
        for(int j=0; j<bits; j++)
        {
            printf("%d ", store[i][j] ) ;
        }
        printf("\n");
    }

    printf("\n");
}

/**
 * Displays the contents of the accumulator
 * */
void displayAccumulator()
{
    for(int i=0; i<bits; i++)
    {
        printf("%d ", accumulator[i]);
    }

    printf("\n");
}

/**
 * Displays the contents of the present instruction
 * */
void displayPresentInstruction()
{
    for(int i=0; i<bits; i++)
    {
        printf("%d ", presentInstruction[i]);
    }

    printf("\n");
}

/*Frees the memory of Store
*@return SUCCESS if memory was freed successfully*/
int freeStore()
{
    for (int i=0; i<bits; i++)
    { 
        free(store[i]); 
    }
    
    free(store);
    return SUCCESS;
}

/* Resets Store to its default values if an error was detected in a machine code while its values were in the
process of being stored in Store
@param row - a number of row at which to start resetting the values of Store in backward manner
*/
void resetStore(int row)
{
    for(int i = row; i >= 0; --i)
    {
        for(int j=0; j< bits; j++)
        {
            store[i][j] = 0;
        }
    }
}
