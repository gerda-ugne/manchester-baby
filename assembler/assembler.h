#ifndef _ASSEMBLER_
#define _ASSEMBLER_

/*ERROR CODES*/

#define SUCCESS                  100
#define INVALID_INPUT_PARAMETER  101
#define MEMORY_ALLOCATION_ERROR  102   
#define INVALID_LIST_OPERATION   103
#define FILE_IO_ERROR            10

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

void createBuffer();
void clearBuffer();
void addToBuffer(char toAdd[]);
void printBuffer();
char* convertToBE(int number);
void firstPass(char lines[256][256]);
void initialiseInstructionSet();
void loadCode(char lines[256][256]);

#endif