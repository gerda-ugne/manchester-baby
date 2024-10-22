/* Group number: 5
*  Team members:
*       Matthew Bain    - 190012264
*       Aidan Dolan     - 120014811
*       Sem Jasaityte   - 190014736
*       Laura Naslenaite- 190013642
*       Gerda Pupelyte  - 190013631
*/
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

/**
 * Runs the Manchester Baby simulator
 * */
void runSimulator()
{
    int fetchStatus = 0;
    int executeStatus = 0;
    int function = 0;

    //While there are lines to fetch and the loop is not halted
    while(fetchStatus!=INVALID_FETCH && executeStatus != -1)
    {
        printf("\nIncrementing Control Instruction... \n");
        incrementCI();
        fetchStatus = fetch();
        function = decode();
        printf("Control instruction: %d \n", controlInstruction);
        printf("FUNCTION: %d, LINE NUMBER(OPERAND): %d \n",function, lineNumber);

        executeStatus = execute(function);

        printf("Store @ Line Number:" );
        displayStoreAtLine();

        printf("\nPresent instruction: ");
        displayPresentInstruction();

        printf("Accumulator: ");
        displayAccumulator();
        printf("\nStore:\n");
        displayStore();
    }

    if(executeStatus == -1) printf("\n STP - The simulation was halted.\n");
    
}

/*Displays content of a store at a chosen line*/
void displayStoreAtLine()
{
     printf("\033[0;32m"); //The start of coloring the output in green
    for(int i=0; i<bits; i++)
    {
        if( store[lineNumber][i] == 0) printf(". " ) ;
        else printf("~ ");
    }
    printf("\033[0m"); //The end of coloring the output
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
    initialiseStore();
    
    return (accumulator==NULL || presentInstruction==NULL || store==NULL ) ? MEMORY_ALLOCATION_ERROR: SUCCESS;
}

/**
 * Frees the memory that has been allocated.
 * */
void freeMemory()
{
    free(negOperand(store[lineNumber]));
    free(accumulator);
    free(presentInstruction);
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
    if(presentInstruction == NULL) return INVALID_PARAMETER;
    if(store == NULL) return INVALID_PARAMETER;

    //If there is nothing to fetch return error
    if(store[controlInstruction]==NULL) 
    {
        //Reset the accumulator
        free(presentInstruction);
        presentInstruction = (int*)calloc(bits, sizeof(int));
        return INVALID_FETCH;
    }
    else
    {
        memcpy(presentInstruction, store[controlInstruction], bits*sizeof(int));
    }


    return SUCCESS;

}

/*Decodes the fetch instruction*/
int decode()
{
	lineNumber=presentInstruction[0]+presentInstruction[1]*2+presentInstruction[2]*4+presentInstruction[3]*8+presentInstruction[4]*16;
	int function =  presentInstruction[13]+presentInstruction[14]*2+presentInstruction[15]*4+presentInstruction[16]*8;

    return function;
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
        if(i==bits-1) number-=(binaryArray[i]*raiseToPower(2,i));
		else number+=(binaryArray[i]*raiseToPower(2,i));
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

/*Executes the decoded instruction, returns -1 if program has ended, or SUCCESS if instruction successfully executed
* param function - instructions
* return SUCCESS if the instruction was processed 
*/
int execute(int function)
{
	switch(function) {
		// Jump to the instruction at the address obtained from the specified memory address at lineNumber
		case 0: //JMP
            printf("MNEMONICS: JMP\n");
			controlInstruction= lineNumber;
			return SUCCESS;
		// Jump to the instruction at controlInstruction plus lineNumber
		case 1: //JRP
            printf("MNEMONICS: JRP\n");
			controlInstruction+=lineNumber;
			return SUCCESS;
		// Load the number from store[lineNumber] to accumulator
		case 2: //LDN
            printf("MNEMONICS: LDN\n");
			//memcpy(accumulator, store[lineNumber], bits*sizeof(int));
            memcpy(accumulator, negOperand(store[lineNumber]), bits*sizeof(int));
			printf("OUTPUT: %d\n", convertBinaryToInt(accumulator));
			return SUCCESS;
		// Stores number in accumulator to store[lineNumber]
		case 3: //STO
            printf("MNEMONICS: STO\n");
			memcpy(store[lineNumber], accumulator, bits*sizeof(int));
			return SUCCESS;
		// Subtracts number at store[lineNumber] from accumulator (practically adds 2 negative numbers: -# -#)
		case 4: //SUB
            printf("MNEMONICS: SUB\n");
            printf("Arithmetic Operation: %d - %d\n", convertBinaryToInt(accumulator), convertBinaryToInt(store[lineNumber]));
            accumulator=sumBinaryNumbers(accumulator, store[lineNumber]);
            printf("OUTPUT:%d\n", convertBinaryToInt(accumulator));
			return SUCCESS;
		// Adds 2 numbers (practically substracts a negative number(value in accumulator) from a positive(value in store): -# + # )
		case 5: //SUM
            printf("MNEMONICS: SUM\n");
            printf("Arithmetic Operation: %d + %d\n", convertBinaryToInt(accumulator), convertBinaryToInt(store[lineNumber]));
			accumulator=subtractBinaryNumbers(accumulator, store[lineNumber]);
            printf("OUTPUT:%d\n", convertBinaryToInt(accumulator));			
			return SUCCESS;
		// If accumulator contains a negative value, skip next instruction
		case 6: //CMP
            printf("MNEMONICS: CMP\n");
			if(convertBinaryToInt(accumulator)<0) controlInstruction++;
			return SUCCESS;
		// Stop program
		case 7: //STP
            printf("MNEMONICS: STP - PROCESS HALTED.\n");
			return -1;
        //multiplication:
        case 8: //MTP
            printf("MNEMONICS: MTP\n");
            printf("Arithmetic Operation: %d * %d\n", convertBinaryToInt(accumulator), convertBinaryToInt(store[lineNumber]));
            multiply(store[lineNumber]);
            printf("OUTPUT:%d\n", convertBinaryToInt(accumulator));         
            return SUCCESS;
        //division:
        case 9: //DVD
            printf("MNEMONICS: DVD\n");
            printf("Arithmetic Operation: %d / %d\n", convertBinaryToInt(accumulator), convertBinaryToInt(store[lineNumber]));
            divide(store[lineNumber]);
            printf("OUTPUT:%d\n", convertBinaryToInt(accumulator));         
            return SUCCESS;
	}

    return -1;
}

/**
 * Sums two binary numbers.
 * @param binary1 - first binary number
 * @param binary2 - second binary number
 * @return pointer to binary answer
 * */
int* sumBinaryNumbers(int* binary1, int* binary2)
{
    int* result = (int*)calloc(bits, sizeof(int));
    int* negatedBinary1 = negOperand(binary1);
    for(int i=0; i<bits; i++)
    {
        result[i] = binary2[i] + negatedBinary1[i] + result[i];
        if (result[i] == 2) 
        {
            result[i] = 0;
            if(i!=bits-1) result[i+1] = 1;
        }
        else if (result[i]==3)
        {
            result[i] = 1;
            if(i!=bits-1) result[i+1] = 1;
        }
    }
    return negOperand(result);
}

/**
 * Subtracts two binary numbers.
 * @param binary1 - first binary number
 * @param binary2 - second binary number
 * @return pointer to binary answer
 * */
int* subtractBinaryNumbers(int* binary1, int* binary2)
{
    int* result = (int*)calloc(bits, sizeof(int));
    for(int i=0; i<bits; i++)
    {
        result[i] = binary1[i] + binary2[i] + result[i];
        if (result[i] == 2) 
        {
            result[i] = 0;
            if(i!=bits-1) result[i+1] = 1;
        }
        else if (result[i]==3)
        {
            result[i] = 1;
            if(i!=bits-1) result[i+1] = 1;
        }
    }
    return result;
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
    printf("\033[0;32m"); //The start of coloring the output in green 
    for(int i =0; i<bits; i++)
    {
        printf("\t");

        for(int j=0; j<bits; j++)
        {
            if(store[i][j] == 0) printf(". " ) ;
            else printf("~ ");
            
        }
        printf("\n");
    }
    printf("\033[0m"); // The end of coloring
    printf("\n");
}

/**
 * Displays the contents of the accumulator
 * */
void displayAccumulator()
{
    printf("\033[0;33m"); //The start of coloring the output in yellow 
    for(int i=0; i<bits; i++)
    {
        if( accumulator[i] == 0) printf(". " ) ;
            else printf("~ ");
    }
    printf("\033[0m"); // The end of coloring
    printf("\n");
}

/**
 * Displays the contents of the present instruction
 * */
void displayPresentInstruction()
{
    printf("\033[0;33m"); //The start of coloring the output in yellow
    for(int i=0; i<bits; i++)
    {
        if( presentInstruction[i] == 0) printf(". " ) ;
            else printf("~ ");
    }
    printf("\033[0m");
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

/*Negates the binary expression while using a 2's complement format
@param array - a pointer to an array which binary expression is being negated 
@return negArray - a pointer to an array with a negated binary expression
*/
int *negOperand(int *array)
{
    int add=1;
    int tempArray[bits];
    int *negArray=(int*)malloc(sizeof(int) * bits);

    for(int i=0; i < bits; i++)
    {
        if(array[i]==0)
            tempArray[i] = 1;
        else
            tempArray[i] = 0;
    }

    /*The principle of an algorithm - adding 1 to a toggled binary number, was based on an example from a resource cited below: 
    *
    * Title: What is the 2s complement in C?
    * Author:javatpoint.com 
    * Date: not stated 
    * Availability: https://www.javatpoint.com/2s-complement-in-c
    *
    * I alterred variable names and loop condition.
    */
    for(int i=0; i < bits; i++)
    {
        //inspecting whether the byte is 1 and  there is an extra value of 1 to be added
        if(tempArray[i]==1 && add == 1)
            negArray[i] = 0;
        //if the slot of that binary digit is 0 and there is a plus 1 needs to be added 
         else if(tempArray[i] == 0 && add==1)
        {
            negArray[i]=1;
            add=0;
        }
        else
            negArray[i] = tempArray[i];
    }
    /*
    * End of code extract. 
    */
    return negArray;
}

/*A function to multiply the number stored in accumulator by a binary number.
*@param array - the binary array holding the multiplier
*/
void multiply(int *array)
{
    //converts the binary number to an integer
    int times = convertBinaryToInt(array);

    //copies the current accumulator value to another array
    int *tempArray = (int*)calloc(bits, sizeof(int));
    memcpy(tempArray, accumulator, bits*sizeof(int));

    //negates the temporary array
    tempArray=negOperand(tempArray);

    //adds the starting value to the accumulator for the given times integer
    for(int i=0;i<times-1;i++)
    {
        accumulator=sumBinaryNumbers(accumulator, tempArray);
    }

    //frees temporary array
    free(tempArray);
}

/*A function to divide the number stored in accumulator by a binary array holding the divisor.
*@param array - the binary array to divide by
*/
void divide(int* array)
{
    //the result variable holds the result of accumulator divided by divisor array
    int result = 0;

    //boolean telling function if the result of the division will be negative
    int resultWillBeNegative=0;

    //copies starting accumulator and divisor values to other arrays
    int *tempArray = (int*)calloc(bits, sizeof(int));
    int *tempDivisor = (int*)calloc(bits, sizeof(int));

    memcpy(tempArray, accumulator, bits*sizeof(int));
    memcpy(tempDivisor, array, bits*sizeof(int));

    if(convertBinaryToInt(tempArray)<0)
    {
        if(convertBinaryToInt(tempDivisor)<0) {
            //if divisor is negative, make it positive
            tempDivisor=negOperand(tempDivisor);
        }
        //if accumulator is negative, make it positive
        tempArray=negOperand(tempArray);
    }

    //if integer values of the two arrays are not both positive or negative, the result will be negative
    if(((convertBinaryToInt(tempArray)<0)&&(convertBinaryToInt(tempDivisor)>0))||((convertBinaryToInt(tempArray)>0)&&(convertBinaryToInt(tempDivisor)<0))) resultWillBeNegative=1;
    
    //while the integer value of the temporary array is bigger than or equal to the divisor array...
    while(compareBinaryNumbers(tempArray,tempDivisor)){
        //...subtract the divisor array from the temporary array
        tempArray = sumBinaryNumbers(tempArray, tempDivisor);
        //increment result
        result++;
    }

    //if the result has to be negative, negate it
    if(resultWillBeNegative) result*=-1; 
    //once the integer value of the temporary array is smaller than the divisor array, copy result to accumulator
    accumulator=intToBinary(result);

    //free temporary arrays
    free(tempArray);
    free(tempDivisor);
}

/*Checks if binary1 is bigger than or equal to binary2, returns 1 if it is bigger and 0 otherwise*/
int compareBinaryNumbers(int* binary1, int* binary2)
{
    if(convertBinaryToInt(binary1)>=convertBinaryToInt(binary2)) return 1;
    return 0;
}

/*Converts integer to a binary number
*@param number - integer to convert
*@return an array containing the binary number
*/
int* intToBinary(int number)
{
    int* result = (int*)calloc(bits, sizeof(int));
    //Calculates a regular binary number from the given integer
    for(int i=0; number>0;i++)
    {
        result[i]=number%2;
        number=number/2;
    }
    //if number is negative, set most significant bit to 1 (using 2s complement)
    if(number<0) result[bits-1]=1;
    return result;
}
