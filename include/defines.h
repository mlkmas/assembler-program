#ifndef UNTITLED6_DEFINES_H
#define UNTITLED6_DEFINES_H


/* File and string handling constants */
#define MAX_LINE_LENGTH 82        /* Max line length including null terminator */
#define MAX_LABEL_LENGTH 31         /* Max length for symbols/labels */
#define MAX_VALUE_LEN 1000        /* Max length for macro values */
#define MAX_FILE_NAME 256         /* Max filename length */

/* Machine architecture constants */
#define WORD_BITS 12             /* Number of bits per machine word */
#define NUM_REGISTERS 9          /* r0-r7 + PSW register */
#define MAX_OPERANDS 2           /* Max operands per instruction */

/* File extensions */
#define AS_EXT ".as"            /* Source file extension */
#define OBJ_EXT ".ob"           /* Object file extension */
#define ENT_EXT ".ent"          /* Entries file extension */
#define EXT_EXT ".ext"          /* Externals file extension */
#define AM_EXT ".am"         /* Macro-expanded file extension */

/* Assembly process constants */
#define INITIAL_IC 100          /* Starting address for instructions */
#define MAX_MEMORY 4096         /* Total available memory (2^12) */

/* Pre Assembler constants */
#define MAX_MACRO_NAME_LEN 50
#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define COMMENT_SYMBOL ';'

#define MAX_WORDS_PER_INSTRUCTION 3


/* */
typedef struct
{
    int word;
    char name[32];/* for extern/intry words in out files */
    char are; /*'A', 'R', or 'E' for the output file*/
} MachineWord;

/*first word srtuct
 * instructions word*/
typedef struct {
     int numOperand; /*the number of the operands*/
     int wordCount; /*the total of the machine words this instruction uses 1-3*/ 
     int funct ;
     int opcode ;
     int address; /*IC valur for the instruction */
     int lineNum; /*store line number for error mssg*/

     /*operands info*/
     int mode[2];
     int reg[2];
     int imm[2];
     char labelName[2][32];/*label name in ces mode is 1or2*/
     MachineWord words[MAX_WORDS_PER_INSTRUCTION];
} Instruction;


/*register word mode 3*/
typedef struct
{
    unsigned int reg_bit : 8;  /*Bits 0-7*/
    unsigned int unused  : 4;  /* Bits 8-11: Always 0*/
} regWord;

/* data words*/
typedef struct
{
    signed int value : 12;
} dataWord;









#endif /*UNTITLED6_DEFINES_H */
