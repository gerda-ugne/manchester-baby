#ifndef _ASSEMBLER_
#define _ASSEMBLER_

/*ERROR CODES*/

#define SUCCESS                  100
#define INVALID_INPUT_PARAMETER  101
#define MEMORY_ALLOCATION_ERROR  102   
#define INVALID_LIST_OPERATION   103
#define FILE_IO_ERROR            10

typedef struct instructionSet{

char stringInstruction[4];
char binaryInstruction[4];


} InstructionSet;
typedef struct listNode
{
	char binary[14];
	struct listNode *next;
} ListNode;


typedef struct linkedList
{
	ListNode *head;
	ListNode *tail;
} LinkedList;

typedef struct tableNode {

    char label[50];
    int value;
	struct tableNode* next;

} TableNode;

typedef struct table
{
	struct tableNode *head;
} Table;

int createBuffer();
void clearBuffer();
int createSymbolTable();
void clearSymbolTable();
void addToBuffer(char toAdd[]);
int addToTable(char* label);
int checkIfInSymbolTable(char* toCheck);
void printBuffer();
void printSymbolTable();
char* convertToBE(int number);
void firstPass(char lines[256][256]);
void initialiseInstructionSet();
int loadCode(char lines[256][256]);
int assignValueToLabel(char* label, int value);

#endif