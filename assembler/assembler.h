#ifndef _ASSEMBLER_
#define _ASSEMBLER_

/*ERROR CODES*/

#define SUCCESS                  100
#define INVALID_INPUT_PARAMETER  101
#define MEMORY_ALLOCATION_ERROR  102   
#define INVALID_LIST_OPERATION   103
#define FILE_IO_ERROR            104

const char* convertToBE(int number);
void firstPass(char lines[][256]);
void initialiseInstructionSet();
void loadCode(char lines[][256]);

#endif