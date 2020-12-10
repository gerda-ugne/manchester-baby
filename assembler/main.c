#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "assembler.h"

/**Displays menu options for a user*/
void display();
/**Processes user's options */
void userChoice();
/**Runs assembler*/
void runAssembler();
/**Gets a valid file name*/
void getFileName(char name[],int size);
/**Validates int variables */
void validateInt( int min, int max, int *number);


/*Runs the main program */
int main()
{
	userChoice();
	
    return 0;
}

/*Displays menu options*/
void display()
{
	printf("\n");
    printf("---ASSEMBLER-----------------------------------------------\n");
    printf("1. Build a Machine Code from a Code in Assembly Language\n");
    printf("0. Exit\n");
    printf("\n");
}

/*Processes user choices*/
void userChoice()
{
    int choice=0;
    do
    {
        display();
        printf("Enter your option:");   
        validateInt(0,1,&choice);
      
        if(choice == 1)
            runAssembler();
    }
    while(choice != 0);
}

/*Runs Assembler*/
void runAssembler()
{ 
    char readLines[256][256];
    int valid = SUCCESS;
    int result=1;

    printf("Enter a valid file name with assembly language instructions (up to 50 characters): ");
    char loadName[52];
    int length = 52;
    getFileName(loadName, length); // updates loadFile variable with an actual file name
    valid=loadCode(readLines, loadName); 

    if(valid != SUCCESS)
    {
        printf("\nFile was not found\n");
        return; //goes back to the main menu
    }
   printf("File name with program in assembly language: %s\n", loadName);

    //Allocate memory for buffer and symbol table
    createBuffer();
	createSymbolTable();
    //Initalize the instruction set
	initialiseInstructionSet();

    //Complete the first pass of the assembler
	firstPass(readLines);	
	
  
    printf("\nMachine code was created successfully!\n");
    printf("To save it, enter a file name (up to 50 characters)\n");
    printf("(Note that an already existing file will be overwritten) : ");

    char saveName[52];
    int length1 = 52;

    do
    {
        //Updates saveName variable with a file name
        getFileName(saveName, length1);
        valid= writeToFile(saveName,32);

        if(valid != SUCCESS)
        {
            printf("\nFailed to open a file.\n");
            printf("\n");
            printf("1. Re-enter the file name\n");
            printf("0. Go back. NOTE- Machine code will not be saved\n");
            printf("Your option: ");
            //Updates 'result' variable with user's choice
            validateInt(0,1,&result);
            if(result == 1)
            {
                result=1;
                printf("Enter a file name where machine code will be saved (up to 50 characters): ");
            }
            else
                result=0;
        }
        else
        {
            printf("\n");
            printf("Machine code was saved in file %s\n", saveName);
            printf("\n");
            result=0;
        }
    }while(result==1);

    //Freeing allocated memory
	clearSymbolTable();
	clearBuffer();
}


/*Gets a name of a file
@param name - an array of characters where name is stored (a pointer)
@param size - a length of an array
*/
void getFileName(char name[],int size)
{  
    int k=0  ;
    char *p;
    do
    { 
        fgets(name, size, stdin);
        if ( (p = strchr(name, '\n')) != NULL)
        {
            *p = '\0';
            k=1;
        }
        else
        {
            while(getchar() != '\n');
        }
    }while(k==0);
}
/*
Makes sure that int is valid and within the range
@param min - a minimum value 
@param max - a maximum value
@param *number - a pointer to a variable
*/
void validateInt( int min, int max, int *number)
{
    int x = 0;
    int i = 0;
    int valid=0;
    do
    {
        if(valid!=0)
        printf("Invalid input. Re-enter: ");
        x = scanf("%d", number);
        if(x == 0 || (x == 1 && getchar() != '\n'))
        {
            i = 0;
            while(getchar() != '\n');
        }
        else
        {
            if( *number >= min && *number <= max)
            {
                i = 1;
            }
        }
        valid++;
    }while(i == 0);

    printf("\n");
}


