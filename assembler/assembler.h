#ifndef _ASSEMBLER_
#define _ASSEMBLER_

/*ERROR CODES*/

#define SUCCESS                  100 //Return if operation was carried out successfully
#define INVALID_INPUT_PARAMETER  101 //Return if the input parameter was invalid 
#define MEMORY_ALLOCATION_ERROR  102 //Return if there was a memory allocation error  
#define INVALID_OPERATION  		 103 //Return if the operation is invalid with given data
#define FILE_IO_ERROR            104 //Return if there was an error with file handling

/**Defines the instruction set for the assembler*/
typedef struct instructionSet{

//Holds the human-readable instruction
char stringInstruction[4];
//Holds the binary equivalent of the instruction
char binaryInstruction[5];


} InstructionSet;

/**Defines a list node for the buffer*/
typedef struct listNode
{
	char binary[14];
	struct listNode *next;
} ListNode;

/**Defines a linked list structure for the buffer*/
typedef struct linkedList
{
	ListNode *head;
	ListNode *tail;
} LinkedList;

/**Defines the table node for symbol table*/
typedef struct tableNode {

	//Label of a symbol
    char label[50];
	//Value of the symbol
    int value;
	//Pointer to next symbol in the table
	struct tableNode* next;

} TableNode;

/**Defines table for the symbol table*/
typedef struct table
{
	struct tableNode *head;

} Table;

/**Initialies the base instructions for the assembler*/
void initialiseInstructionSet();
/**Allocates memory for the buffer list*/
int createBuffer();
/**Releases the memory for the buffer list*/
void clearBuffer();
/**Allocates memory for the symbol table*/
int createSymbolTable();
/**Releases the memory for the symbol table*/
void clearSymbolTable();
/**Adds an input to buffer*/
void addToBuffer(char toAdd[]);
/**Adds a new label to the symbol table*/
int addToTable(char* label);
/**Checks whether the label exists in the symbol table*/
int checkIfInSymbolTable(char* toCheck);
/**Assigns value to found label in the symbol table*/
int assignValueToLabel(char* label, int value);
/**Prints the output buffer*/
void printBuffer();
/**Prints the symbol table*/
void printSymbolTable();
/**Converst binary to big endian*/
char* convertToBE(int number);
/**Completes the first pass of the assembler*/
void firstPass(char lines[256][256]);
/**Loads the input code into the assembler*/
int loadCode(char lines[256][256]);
/**Writes the binary output to a text file*/
int writeToFile(char* fileName, int bits);

#endif