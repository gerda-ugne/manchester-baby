#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"


/*Holds the instruction set for the assembler*/
InstructionSet instructions[11];
/*Stores memory adresses of user's labels and variables*/
Table* symbolTable;
/*Holds the code output*/
LinkedList *buffer;

/**
 * Allocate memory for the symbol table.
 * @return MEMORY_ALLOCATION_ERROR is malloc fails,
 * else return SUCCESS
 * */
int createSymbolTable()
{
	symbolTable = (Table*)malloc(sizeof(Table));
	if(symbolTable == NULL) return MEMORY_ALLOCATION_ERROR;

	//initialise the fields to NULL
	symbolTable->head = NULL;
	return SUCCESS;
}

/**
* Creates a new code output buffer
* @return MEMORY_ALLOCATION_ERROR is malloc fails,
* else return SUCCESS
*/
int createBuffer()
{
	//assign memory space to store the buffer
	buffer = (LinkedList*)malloc(sizeof(LinkedList));
	if(buffer == NULL) return MEMORY_ALLOCATION_ERROR;

	//initialise the fields to NULL
	buffer->head = NULL;
	buffer->tail = NULL;

	return SUCCESS;
}

/**
 * Frees the memory for the symbol table.
 * */
void clearSymbolTable()
{
	//while the buffer is not empty
	while(symbolTable->head != NULL)
	{
		//create a pointer to the head of the buffer
		TableNode *temp = symbolTable->head;

		//set the head to the following node
		symbolTable->head = symbolTable->head->next;

		//free the memory location of the last node
		free(temp);
	}

	free(symbolTable);
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

/**
* Add a new node to the buffer
* @param toAdd - value to be added
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
* Prints the symbol table to the terminal
*/
void printSymbolTable()
{
	TableNode* current = symbolTable->head;
	if(symbolTable->head == NULL) printf("Table is empty.\n");

	//loop until there is no next node
	while(current != NULL)
	{
		//print the current node
		printf("LABEL: %s, VALUE: %d\n", current->label, current->value);
		//move to the next node
		current = current->next;
	}

	printf("\n");
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
	strcpy(instructions[0].binaryInstruction, "0000");

	strcpy(instructions[1].stringInstruction, "JRP");
	strcpy(instructions[1].binaryInstruction, "1000");

	strcpy(instructions[2].stringInstruction, "LDN");
	strcpy(instructions[2].binaryInstruction, "0100");

	strcpy(instructions[3].stringInstruction, "STO");
	strcpy(instructions[3].binaryInstruction, "1100");

	strcpy(instructions[4].stringInstruction, "SUB");
	strcpy(instructions[4].binaryInstruction, "0010");

	strcpy(instructions[5].stringInstruction, "SUB");
	strcpy(instructions[5].binaryInstruction, "1010");

	strcpy(instructions[6].stringInstruction, "CMP");
	strcpy(instructions[6].binaryInstruction, "0110");

	strcpy(instructions[7].stringInstruction, "STP");
	strcpy(instructions[7].binaryInstruction, "1110");

	strcpy(instructions[8].stringInstruction, "VAR");
	strcpy(instructions[8].binaryInstruction, "   ");

	strcpy(instructions[9].stringInstruction, "MTP");
	strcpy(instructions[9].binaryInstruction, "0001");

	strcpy(instructions[10].stringInstruction, "DVD");
	strcpy(instructions[10].binaryInstruction, "1001");


	/*
	* DEBUG CODE: Prints all the opcodes and binary equivalents
	
	for(int i = 0; i < 10; i++)
	{
		printf("%s : %s\n", instructions[i].stringInstruction, instructions[i].binaryInstruction);
	}*/
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
	//Line number is the iterator for the lines of the
	//code that have been passed
	int lineNumber = 0;

	//the following will loop until the end of
	//line character is the first character in
	//a line
	while(lines[lineNumber][0] != '\0')
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
		if(lines[lineNumber][0] != ';')
		{
			//split the line about the delimiter
			str = strtok(lines[lineNumber], delimiter);

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

							//check if the variable is already in the symbol table
							if(split[0] != NULL)
							{	

								//array to store commaless label
								char noCol[10];
								//iterator for current label
								int k = 0;
								//iterator for new label
								int l = 0;

								//while the string is not finished
								while(split[0][k] != '\0')
								{
									//if the character at the current index
									//is not a comma
									if(split[0][k] != ':')
									{
										//copy the character to the new array
										noCol[l] = split[0][k];
										//move to the next slot of the new
										//array
										l++;
									}

									//move to the next character of the
									//label
									k++;
								}

								//finish the new label with the string
								//end character
								noCol[l] = '\0'; 
							
								if(checkIfInSymbolTable(split[0]) == 0)
								{

									//If label not found, add it to the table
									addToTable(noCol);
									int intValue=atoi(split[2]);

									assignValueToLabel(noCol, intValue);

								}
							
								printf("SYMBOL TABLE\n");
								printSymbolTable();
							}

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
				else
				{
					//If no operand present, fill the space anyway
					addToBuffer("0000");
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
		lineNumber++;

		/*
		* DEBUG CODE: Print the values in the array
		*/
		for(int k = 0; k < 3; k++)
		{
			printf("%d: %s ", k, split[k]);
		}
		printf("\n");

	}

	/*
	* DEBUG CODE: Display the buffer
	*/ 
	printBuffer();

}

void secondPass(char lines[256][256])
{
	//Line number is the iterator for the lines of the
	//code that have been passed
	int lineNumber = 0;

	//the following will loop until the end of
	//line character is the first character in
	//a line
	while(lines[lineNumber][0] != '\0')
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
		if(lines[lineNumber][0] != ';')
		{
			//split the line about the delimiter
			str = strtok(lines[lineNumber], delimiter);

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

				//if there is an operand
				if(split[2] != NULL)
				{

					//if the operand is not numberic, it must be
					//an undeclared variable
					if(isdigit(split[2][0]) == 0)
					{
						
						int found = 0;

						//start at the head of the buffer
						ListNode *pNext = buffer->head;

						//start at the head of the table
						TableNode *tNext = symbolTable->head;

						//loop until there is no next label in the table
						while(tNext != NULL)
						{
							//if the label matches the found operand
							if(strcmp(tNext->label, split[2]) == 0)
							{
								//set the found variable to the value
								found = tNext->value;
								//break the loop
								break;
							}
							//move to the next node
							tNext = tNext->next;
						}

						//loop until there is no next entry in the buffer
						while(pNext != NULL)
						{
							//if the space in the buffer is empty
							if(strcmp(pNext->binary, "") == 0)
							{
								//insert the value in binary into the slot
								strcpy(pNext->binary, convertToBE(found));
								//break the loop
								break;
							}

							//move to next node
							pNext = pNext->next;
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
		lineNumber++;
		/*
		* DEBUG CODE: Print the values in the array
		*/
		for(int k = 0; k < 3; k++)
		{
			printf("%d: %s ", k, split[k]);
		}
		printf("\n");

	}

	/*
	* DEBUG CODE: Display the buffer
	*/ 
	printBuffer();
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
	for(int i = 12; i >= 0; i--)
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
int loadCode(char lines[256][256])
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
		return SUCCESS;

	}
	else return FILE_IO_ERROR;
}

/**
 * Checks if the string is currently in the symbol table.
 * @param toCheck - label to find
 * @return 1 if found, 0 if not found
 * */
int checkIfInSymbolTable(char* toCheck)
{
	TableNode* current = symbolTable->head;
	while(current!=NULL)
	{
		if(strcmp(current->label,toCheck) == 0)
		{
			return 1;
		}

		current = current->next;
	}

	free(current);
	return 0;
}

/**
 * Adds a new label to the symbol table.
 * @param label - label to be added
 * @return SUCCESS or MEMORY_ALLOCATION_ERROR
 * */
int addToTable(char* label)
{
	TableNode* toAdd = (TableNode*)malloc(sizeof(TableNode));
	if(toAdd == NULL) return MEMORY_ALLOCATION_ERROR;

	strcpy(toAdd->label, label);
	toAdd->next = NULL;

	if(symbolTable->head == NULL)
	{
		symbolTable->head = toAdd;
	}
	else
	{
		//iterate to the end of the list
		TableNode* current = symbolTable->head;
		while(current->next!=NULL)
		{
			current=current->next;
		}

		current->next = toAdd;

		return SUCCESS;
	}

	return SUCCESS;

}

/**
 * Assigns the value to the found label.
 * @param label - label to look for
 * @param value - value to be assigned
 * @return SUCCESS if value changed, INVALID_INPUT_PARAMETER if label doesn't exist.
 * */
int assignValueToLabel(char* label, int value)
{
	TableNode* current = symbolTable->head;

	//Iterate through list and find where to insert the value
	while(current!=NULL)
	{
		if(strcmp(current->label,label) == 0)
		{
			//Adjust the value of the entry
			current->value = value;
			return SUCCESS;
		}

		current = current->next;
	}
	//If label not found
	return INVALID_INPUT_PARAMETER;

}
/**
 * Writes the binary output of the buffer to a text file.
 * @param fileName - name of the file to write to.
 * @return SUCCESS if written, FILE_IO_ERROR if there was a problem with a file
 * */
int writeToFile(char* fileName, int bits)
{
	FILE *fp;
	fp = fopen(fileName, "w");
	//If the file does not exist
	if(fp == NULL) return FILE_IO_ERROR;

	ListNode* bufferLine = buffer->head;
	//If buffer is empty
	if(bufferLine == NULL) return INVALID_OPERATION;

	/** START OF THE FILE*/
	//Print 32 0s if assembling for 32bit system
	if(bits == 32)	fprintf(fp, "%s\n", "00000000000000000000000000000000");
	//Print 64 0s if assembling for 64bit system
	else if (bits == 64) fprintf(fp, "%s\n", "0000000000000000000000000000000000000000000000000000000000000000");
	{

	}

	/** Counter tracks which part of the line is being fetched
	 * 0 - instruction
	 * 1 - operand
	*/
	int counter = 0;
	char* instruction = (char*)malloc(sizeof(char)*5);
	//While there are lines to write
	while(bufferLine != NULL)
	{
		//On first iteration we receive the instruction set 
		if(counter == 0)
		{
			//As instruction comes first, we copy it to a variable
			strcpy(instruction, bufferLine->binary);
			counter++;

			//Progress to next line
			bufferLine = bufferLine->next;
		}
		//On second iteration, we read the operand(line number)
		else if(counter == 1)
		{	
			
			//We print the operand first - 0 to 4
			fprintf(fp, "%s", bufferLine->binary);
			//After the operand is printed, fill the space until we can print instruction - 5 to 12
			fprintf(fp, "%s", "00000000");
			//Then we print the instruction to the file
			fprintf(fp, "%s", instruction);
			
			//Then we have to fill the remaining space of the line.
			//Print 15 0s if assembling for 32bit system
			if(bits == 32)	fprintf(fp, "%s\n", "000000000000000");
			//Print 50 0s if assembling for 64bit system
			else if (bits == 64) fprintf(fp, "%s\n", "000000000000000000000000000000000000000000000000");

			//Reset the counter
			counter = 0;

			//Progress to next line
			bufferLine = bufferLine->next;
		}
		

	}

	/** END OF THE FILE*/
	//Print 32 0s if assembling for 32bit system
	if(bits == 32)	fprintf(fp, "%s\n", "00000000000000000000000000000000");
	//Print 64 0s if assembling for 64bit system
	else if (bits == 64) fprintf(fp, "%s\n", "0000000000000000000000000000000000000000000000000000000000000000");

	fclose(fp);
	return SUCCESS;
	
}