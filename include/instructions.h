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
char name[32];
    uint32_t word :24;
} MachineWord;

typedef struct {
    InstrucOp *inst;
    short address;//inst address
    Operand operands[MAX_OPERANDS];
    int wordCount;
    MachineWord words[3];
} Instruction;






int isInstruction(char *name);
int isReg(char *name);
//4 if its entry 3 extern, 1 data 2 string 0 none
int isDirective(const char* name);
int setInstOp(Instruction *instruc, const char *token);
int addressingOps(Instruction *instruc);
#endif //UNTITLED6_INSTRUCTIONS_H
