#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

typedef struct instructionSet
{
	char stringInstruction[4];
	char binaryInstruction[4];
} InstructionSet;

typedef struct symbolTable
{
	char label[14];
	char address[14];
} SymbolTable;

/**/
InstructionSet instructions[9];
int codeBuffer[8];
/*Stores memory adresses of user's labels and variables*/
SymbolTable symbols[8];
/*Loads and runs object code*/
int loader;

LinkedList *buffer;

/*
* Creates a new code output buffer
*/
void createBuffer()
{
	//assign memory space to store the buffer
	buffer = (LinkedList*)malloc(sizeof(LinkedList));

	//initialise the fields to NULL
	buffer->head = NULL;
	buffer->tail = NULL;
}

/*
* Clear the buffer and free the memory
*/
void clearBuffer()
{
	//while the buffer is not empty
	while(buffer->head != NULL)
	{
		//create a pointer to the head of the buffer
		ListNode *temp = buffer->head;

		//set the head to the following node
		buffer->head = buffer->head->next;

		//free the memory location of the last node
		free(temp);
	}

	//set the tail of the buffer to nothing
	buffer->tail = NULL;

	//free the memory location of the buffer
	free(buffer);
}

/*
* Add a new node to the buffer
*/
void addToBuffer(char toAdd[])
{
	//assign the memory for the new node in the buffer
	ListNode *newNode = (ListNode*)malloc(sizeof(ListNode));

	//copy the value to be added to the new node
	strcpy(newNode->binary, toAdd);
	//set the next node to null
	newNode->next = NULL;

	//if there is no tail of the buffer
	//assume it's empty
	if(buffer->tail == NULL)
	{
		//set the new node to both the head and
		//tail of the buffer
		buffer->tail = newNode;
		buffer->head = newNode;
	}
	//otherwise
	else
	{
		//add the new node to the tail of the buffer
		buffer->tail->next = newNode;
		buffer->tail = newNode;
	}
}

/*
* Prints the buffer to the terminal
*/
void printBuffer()
{
	//start at the head of the buffer
	ListNode *pNext = buffer->head;

	//loop until there is no next node
	while(pNext != NULL)
	{
		//print the current node
		printf("%s\n", pNext->binary);
		//move to the next node
		pNext = pNext->next;
	}
}

/*
* Initialises the instruction set
*
* 0 - 7 are opcodes
* 8 is a variable
* 
* stringInstruction[0] would hold the opcode as a string
* binaryInstruction[0] would hold the opcode as big endian binary
*/
void initialiseInstructionSet()
{
	strcpy(instructions[0].stringInstruction, "JMP");
	strcpy(instructions[0].binaryInstruction, "000");

	strcpy(instructions[1].stringInstruction, "JRP");
	strcpy(instructions[1].binaryInstruction, "100");

	strcpy(instructions[2].stringInstruction, "LDN");
	strcpy(instructions[2].binaryInstruction, "010");

	strcpy(instructions[3].stringInstruction, "STO");
	strcpy(instructions[3].binaryInstruction, "110");

	strcpy(instructions[4].stringInstruction, "SUB");
	strcpy(instructions[4].binaryInstruction, "001");

	strcpy(instructions[5].stringInstruction, "SUB");
	strcpy(instructions[5].binaryInstruction, "101");

	strcpy(instructions[6].stringInstruction, "CMP");
	strcpy(instructions[6].binaryInstruction, "011");

	strcpy(instructions[7].stringInstruction, "STP");
	strcpy(instructions[7].binaryInstruction, "111");

	strcpy(instructions[8].stringInstruction, "VAR");
	strcpy(instructions[8].binaryInstruction, "   ");

	/*
	* DEBUG CODE: Prints all the opcodes and binary equivalents
	*/
	for(int i = 0; i < 9; i++)
	{
		printf("%s : %s\n", instructions[i].stringInstruction, instructions[i].binaryInstruction);
	}
}

/*
* The first pass of the assembler through the code.
* This will parse the code, store the opcodes and
* operands in the output buffer, and create a symbol
* table
* The passed array holds the lines of code
*/
void firstPass(char lines[256][256])
{
	//i is the iterator for the lines of the
	//code that have been passed
	int i = 0;

	//create output code buffer
	createBuffer();

	//the following will loop until the end of
	//line character is the first character in
	//a line
	while(lines[i][0] != '\0')
	{
		/*
		* The delimiter is comprised of several elements.

		* It will be either:
		* - a space
		* - a new line
		* - a return command
		* - a tab
		*/
		char delimiter[] = " \n\t\r";

		//j will be the iterator for the split string
		//array
		int j = 0;

		//pointer to store the parts of the split
		//string
		char *str;

		/*
		* The split parts will be stored as follows:
		* 
		* split[0] - Label
		* split[1] - Opcode
		* split[2] - Operand
		*/
		char *split[3];

		//if the line starts with a ;, that indicates
		//the line is a comment, so we can skip over
		//this line
		if(lines[i][0] != ';')
		{
			//split the line about the delimiter
			str = strtok(lines[i], delimiter);

			//store the first result to the array
			split[j] = str;

			//loop through all of the instruction set
			for(int k = 0; k < 9; k++)
			{
				//if the first string matches any of the instruction
				//set, it must be an opcode
				if(strcmp(split[j], instructions[k].stringInstruction) == 0)
				{
					//clear the first slot
					split[j] = NULL;
					//store the opcode in the second slot
					split[1] = str;
					//set the counter so that the next slot
					//will be 2
					j = 1;
					break;
				}
			}

			//increment iterator
			j++;

			//while the split array is not full
			while(j < 3)
			{
				//continue splitting the line about the delimiter
				str = strtok(NULL, delimiter);

				//if the split string is not a ;
				if(strcmp(str, ";") != 0)
				{
					//store the string in the array
					split[j] = str;
				}
				//otherwise
				else
				{
					//set the array location to NULL
					split[j] = NULL;
				}
				
				//increment iterator
				j++;
			}

			//if an opcode is present
			if(split[1] != NULL)
			{

				//loop through all the known opcodes
				for(int k = 0; k < 9; k++)
				{
					//if the split opcode matches one of the opcodes
					if(strcmp(split[1], instructions[k].stringInstruction) == 0)
					{
						//the opcode should only be written to
						//the buffer if it is not "VAR"
						if(k != 8)
						{
							//if the opcode is 5, it is a duplicate of 4
							//and should be ignored
							if(k != 5)
							{
								//write the binary of the opcode to the buffer
								addToBuffer(instructions[k].binaryInstruction);
							}
						}
					}
				}

				//if there is an operand
				if(split[2] != NULL)
				{

					//if the operand is not numberic, it must be
					//an undeclared variable
					if(isdigit(split[2][0]) == 0)
					{
						//leave a blank space in the buffer for
						//the variable
						addToBuffer("");
					}
					//otherwise
					else
					{
						//if the opcode was a variable
						if(strcmp(split[1], "VAR") == 0)
						{
							//TODO: add to symbol table
						}
						//otherwise
						else
						{
							//convert the operand to big endian binary
							char converted = *convertToBE(atoi(split[2]));
							//store the operand in the buffer
							addToBuffer(&converted);
						}
					}

				}

			}

		}
		else
		{
			//since the comment line can be ignored
			//fill the current split line with NULL
			//pointers
			for(int k = 0; k < 3; k++)
			{
				split[k] = NULL;
			}
		}

		//move to next line of code
		i++;

		/*
		* DEBUG CODE: Print the values in the array
		*/
		for(int k = 0; k < 3; k++)
		{
			printf("%d: %s\n", k, split[k]);
		}

	}

	/*
	* DEBUG CODE: Display the buffer
	*/ 
	printBuffer();

	/*
	* DEBUG CODE: Clear the memory taken by the list
	*
	* NOTE: REMOVE THIS WHEN PASS 2 IS ADDED
	*/ 
	clearBuffer();
}

/*
/ Convert passed number into big endian binary 
/ and store it in the passed array
*/
char* convertToBE(int number)
{
	//array to be filled with the big endian
	//number and then returned
	static char converted[14];

	//the final character will always be the
	//string end character '\0'
	converted[13] = '\0';

	//the highest binary column will always be 
	//4096 as there are only 12 possible digits
	//for the binary number to be stored in
	int binCol = 4096;

	//loop down through the array until the number
	//becomes 0
	for(int i = 12; number > 0; i--)
	{
		//store the result of the passed number
		//minus the current binary column
		int result = number - binCol;

		//if the result is a number greater than or 
		//equal to 0, the number must fit into that
		//column and should be represented with a 1
		if(result >= 0)
		{
			converted[i] = '1';
			//since the calculation worked and was in range,
			//subtract the column from the passed number
			number -= binCol;
		}
		//otherwise, the number should be represented
		//with a 0
		else
		{
			converted[i] = '0';
		}
		//move to the next binary column by halving
		//the number
		binCol /= 2;
	}	

	return converted;

}

/*
* Function to load in the code from a text file.
* The function is passed an array where the lines
* from the file will be stored.
*/
void loadCode(char lines[256][256])
{
	FILE *code;

	code = fopen("BabyTest1-Assembler.txt", "r");

	if(code != NULL)
	{
		int currentLine = 0;

		while(fgets(lines[currentLine], 256, code) != NULL)
		{

			currentLine++;

		}

		fclose(code);

	}
	else
	{
		//display error to user
		printf("ERROR: File failed to open\n");
	}
}