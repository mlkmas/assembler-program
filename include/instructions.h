#ifndef UNTITLED6_INSTRUCTIONS_H
#define UNTITLED6_INSTRUCTIONS_H
#include <stdint.h>
#include "defines.h"

typedef struct
{
    const char name[10]; // Register name (e.g., "r0")
    short value;        // Corresponding value (e.g., 0 for r0)
} RegMap;

typedef struct {
    const char *name;
    short opcode;
    short numOperands;

} InstrucOp;
typedef enum {
    OP_REGISTER,   // r0-r7
    OP_IMMEDIATE,  // #5
    OP_LABEL       // LOOP or &LOOP
} OperandType;

typedef enum {
    MODE_DIRECT,     // LOOP (label)
    MODE_IMMEDIATE,  // #5
    MODE_RELATIVE    // &LOOP
} AddressingMode;

typedef struct
{
    OperandType type;    // REGISTER/IMMEDIATE/LABEL
    AddressingMode mode; // Only used if type=LABEL
    union
    {
        short reg;     // For registers (0-7)
        int imm;   // For immediates (#-5)
        char label[MAX_LINE_LENGTH];  // For labels ("LOOP")
    };
} Operand;

typedef struct
{
    char name[8];        // "mov", "add" etc.
    short opcode;
    short funct;
    short num_operands;
    Operand operands[MAX_OPERANDS];
} Instruction;

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




int isInstruction(char *name);
int isReg(char *name);
//4 if its entry 3 extern, 1 data 2 string 0 none
int isDirective(char* name);

#endif //UNTITLED6_INSTRUCTIONS_H
