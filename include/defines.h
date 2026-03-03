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



/*first word srtuct
 * instructions word*/
typedef struct {
    unsigned int dstAdd : 2; /*bits 0-1*/
    unsigned int srcAdd  : 2; /* Bits2-3*/
    unsigned int funct    : 4; /*bits 4-7*/
    unsigned int opcode   : 4; /*bits8-11*/
} instruction;


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

/* */
typedef struct
{
    union
    {
        instruction inst;
        regWord reg;
        dataWord data;
    } word;

    char are; /*'A', 'R', or 'E' for the output file*/
} MachineWord;







#endif /*UNTITLED6_DEFINES_H */
