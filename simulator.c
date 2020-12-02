#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
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
    //(void)instructionSet;

    // allocateMemory();
    // runSimulator(store);

    //Testing machine's memory with capacity of 32x32 bits
    initialiseStore();
    displayStore();
    printf("\n");
    fillStore("BabyTest1-MC.txt");
    displayStore();
    freeStore();

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
/*Initialises  Store to its default values 0 
@return SUCCESS if initialization was successful otherwise MEMORY_ALLOCATION_ERROR was detected
*/
int initialiseStore()
{
    store = (int**)malloc(sizeof(int*)*bits);
    if(store == NULL)
        return MEMORY_ALLOCATION_ERROR;
    for(int i =0; i<bits; i++)
    {
        store[i]=(int*)malloc(sizeof(int)*bits);
        if(store[i] == NULL)
            return MEMORY_ALLOCATION_ERROR;
        for(int j=0; j<bits; j++)
        {
            store[i][j] = 0;
        }
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
        int len=strlen(line);

        //Validation: checking if a program is not 64bits when the memory can hold only 32bits
        if(bits == 32 && len > 60)
        { 
            resetStore(counter);
            return INVALID_MACHINE_CODE;
        }
        
        //Making sure that if memory is of 64 bits, a program of 32 bits could be stored 
        int index=bits;
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
process of being stored in Store*/
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
