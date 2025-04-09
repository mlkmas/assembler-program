#ifndef UNTITLED6_INSTRUCTIONS_H
#define UNTITLED6_INSTRUCTIONS_H
#include <stdint.h>
#include "defines.h"

typedef struct
{
    const char name[5]; // Register name (e.g., "r0")
    short value;        // Corresponding value (e.g., 0 for r0)
} RegMap;

typedef struct
{
    char symbol[32]; //in case its associated with a symbol /TO DO: DO I NEED IT? IF YES SHOULD CHNGE TO A SYMBOL PTR?
    int *nums;//for .data
    char *str;//for .string
    int len;
    int isData;//1 if its data, 0= string

} Directive;

typedef struct {
    const char *name;
    short opcode;
    short numOperands;
    short funct;

} InstrucOp;

typedef enum {
    MODE_IMMEDIATE=0,  // #5
    MODE_DIRECT=1,     // LOOP (label)
    MODE_RELATIVE=2,    // &LOOP
    OP_REGISTER=3,   // r0-r7

} AddressingMode;

typedef struct {
    AddressingMode mode;
      short reg;     // For registers (0-7)
      int imm;   // For immediates (#-5)
     short labelAddress; //
     char labelName[31];

} Operand;
typedef struct
{
    uint32_t opcode :6;
    uint32_t funct :5;
    uint32_t rd :3;
    uint32_t rs :3;
    uint32_t src_mode :2;
    uint32_t dest_mode :2;
    uint32_t A :1;
    uint32_t R :1;
    uint32_t E :1;
} MachineWord;

typedef struct {
    InstrucOp *inst;
    short address;//inst address
    Operand operands[MAX_OPERANDS];
    int wordCount;
    MachineWord words[3];
} Instruction;


/* Structure to represent code or data */
typedef struct
{
    int address;         // Address line in memory
    uint32_t mw;           // machine word
} codeConv;

/* Structure to represent other tables ( externs, entries) */
typedef struct {
    char name[MAX_LINE_LENGTH];  // Name of the label
    int address;                 // Address of the label
} otherTable;




int isInstruction(char *name);
int isReg(char *name);
//4 if its entry 3 extern, 1 data 2 string 0 none
int isDirective(const char* name);
int setInstOp(Instruction *instruc, const char *token);
int addressingOps(Instruction *instruc);
#endif //UNTITLED6_INSTRUCTIONS_H
