#include <stdio.h>
#include <string.h>
#include "assembler.h"


typedef struct instructionSet{

char stringInstruction[4];
char binaryInstruction[4];

} InstructionSet;

/**/
InstructionSet instructions[9];
int codeBuffer[8];
/*Stores memory adresses of user's labels and variables*/
int symbolTable[8];
/*Loads and runs object code*/
int loader;

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

	strcpy(instructions[7].stringInstruction, "VAR");
	strcpy(instructions[7].binaryInstruction, "000");

	for(int i = 0; i < 9; i++)
	{
		printf("%s : %s\n", instructions[i].stringInstruction, instructions[i].binaryInstruction);
	}
}

void firstPass(char lines[][256])
{
	int i = 0;

	while(lines[i][0] != '\0')
	{
		char delimiter[] = " \n\t\r";
		int j = 0;
		char *str;
		char *split[3];
		if(lines[i][0] != ';')
		{
			str = strtok(lines[i], delimiter);

			split[j] = str;

			for(int k = 0; k < 9; k++)
			{
				if(strcmp(split[j], instructions[k].stringInstruction) == 0)
				{
					split[j] = NULL;
					split[1] = str;
					j = 1;
					break;
				}
			}

			j++;

			while(str != NULL)
			{
				str = strtok(NULL, delimiter);
				split[j] = str;
				j++;
			}

		}
		else
		{
			for(int k = 0; k < 3; k++)
			{
				split[k] = NULL;
			}
		}

		for(int k = 0; k < 3; k++)
		{
			printf("%d: %s\n", k, split[k]);
		}

		i++;

	}
}

/*
/ Convert passed number into binary 
/ and store it in the passed array
*/
const char* convertToBE(int number)
{
	/*
	/ binary calculations worked out by
	/ checking if deducting each of the
	/ binary columns will result in a
	/ number above 0. If so, this number
	/ will be a 1 in the binary 
	/ representation otherwise, it will be a 0
	*/
	static char converted[14];

	converted[13] = '\0';
	int binCol = 4096;
	for(int i = 12; number > 0; i--)
	{
		int result = number - binCol;
		if(result >= 0)
		{
			converted[i] = '1';
			number -= binCol;
		}
		else
		{
			converted[i] = '0';
		}
		binCol /= 2;
	}	

	return converted;

}

void loadCode(char lines[][256])
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