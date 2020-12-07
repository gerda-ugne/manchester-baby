#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "simulator.h"
#include "simulator.c"

/** Prints the welcome message*/
void printHeader();
/** Displays the menu options to the user*/
void displayMenu();
/** Procceses the user choices*/
void processUserChoices();
/**Collects user's input for a valid file name*/
char* getFileName();
/**Collects user's input within the provided range*/
void collectInteger(int min, int max, int *number);

int main()
{
    printHeader();
    processUserChoices();

    return 0;
}

/** Prints the welcome message to the user*/
void printHeader()
{
    printf("-------------------------------------------\n");
    printf("Welcome to the Manchester Baby Simulator.\n");
}

/** Displays the menu options to the user*/
void displayMenu()
{
    printf("-------------------------------------------\n");
    printf("\n1. Start the simulation \n");
    printf("2. Adjust the system's memory\n");
    printf("0. Exit \n");
}


/** Procceses the user choices*/
void processUserChoices()
{
    int choice = -1;
    while(choice!=0)
    {
        displayMenu();
        collectInteger(0,2,&choice);

        switch (choice)
        {
        case 1:

            printf("\nPREPARING THE SIMULATOR...\n");
            printf("\nALLOCATING MEMORY...\n");
            if(allocateMemory() == MEMORY_ALLOCATION_ERROR)
            {
                printf("Memory allocation error has occured. Please try again\n");
                break;
            }
            else
            {
                printf("MEMORY ALLOCATED SUCCESFULLY!\n");
                printf("\n---------LOOKING FOR THE STORE FILE---------\n");

                printf("WARNING: 64-bit store cannot be supported on 32-bit system.\nIf you would like to run a 64-bit store, please change the system parameters first.\n");

                char name[52];
                int size=52;
                char *fileName = getFileName(name,size);

                if(fillStore(fileName) == FILE_IO_ERROR)
                {
                    printf("No file with the provided name found.\n");
                    break;
                }

                if(fillStore(fileName) == INVALID_MACHINE_CODE)
                {
                    printf("There has been an error processing the machine code.\n Please ensure the provided code matches the system bits and the input file is not corrupt.");
                    break;
                }

                printf("\n---------LAUNCHING THE SIMULATOR---------\n");
                runSimulator();

                freeMemory();
            }

        break;
        case 2:

            printf("\n------ADJUSTING THE SYSTEM BITS------\n");
            printf("\nYou can choose between 32-bit and 64-bit system.\n");
            printf("Please note that whereas 32-bit simulation can be supported on 64-bit system,\nit does not work vice-versa.\n");

            int bitChoice = 0;
            int scanResult = -1;
            while((bitChoice!=32 && bitChoice!=64) || scanResult == EOF)
            {
                //Flush input buffer in case of wrong input
                if(scanResult == 0)  while (fgetc(stdin) != '\n');
                printf("\nChoose the preferred system bits (64 or 32):");
                scanResult = scanf("%d", &bitChoice);
            }

            //Flush input buffer after
            while (fgetc(stdin) != '\n');

            changeBits(bitChoice);
            printf("\nYou have successfully changed the bits. The system runs in %d bit now.\n", bits);

        break;
        case 0:
            printf("Goodbye.");
            return;
        
        default:
            printf("The input is invalid. Please try again.\n");
            break;
        }
    }
}


/**
 * Collects an integer from the user and validate sit.
 * @param min - minimum value allowed
 * @param max - maximum value allowed
 * @param number - pointer to the number variable
*/
void collectInteger(int min, int max, int *number)
{
    int scanResult = -1;
    int input = -1;
    while((input < min && input > max) || scanResult == EOF)
    {
            printf("\nEnter your option:");
            scanResult = scanf("%d", &input);
            //Flush input buffer in case of wrong input
            if(scanResult == 0)  while (fgetc(stdin) != '\n');
    } 

    *number = input;
    while (fgetc(stdin) != '\n');
}


/**
* Gets a valid string
* @param name[] an array of characters to be filled
* @param size - a size of an array 
*
* @return valid string
*/
char *getFileName(char name[],int size)
{  
    int j=0;
    char *p;
    do
    { 
        printf("Please enter the name of the text file to load into the store (up to 52 characters):");
        fgets(name, size, stdin);
        if ( (p = strchr(name, '\n')) != NULL)
        {
            *p = '\0';
            j=1;
        }
        else
        {
            while(getchar() != '\n');
            printf("\nInvalid input. Please try again.\n");
        }
    } while(j==0);

 return name;
}
