#ifndef UNTITLED6_INSTRUCTIONS_H
#define UNTITLED6_INSTRUCTIONS_H
#include <stdint.h>
#include "defines.h"
typedef struct {
    uint32_t opcode    : 6;
    uint32_t funct     : 5;
    uint32_t Rd   : 3;
    uint32_t Rs   : 3;
    uint32_t srcMode   : 2;
    uint32_t destMode  : 2;
    uint32_t A         : 1;
    uint32_t R         : 1;
    uint32_t E         : 1;
    uint32_t operandsNum :2;

} InstrucParts;

typedef struct {
    const char *name; // Register name (e.g., "r0")
    short value;        // Corresponding value (e.g., 0 for r0)
} RegMap;

typedef struct {
    const char *name;
    short opcode;
    short numOperands;

} Instruc;

int isInstructuion(char *name);
int isReg(char *name);
//4 if its entry 3 extern, 1 data 2 string 0 none
int isDirective(char* name);

#endif //UNTITLED6_INSTRUCTIONS_H
