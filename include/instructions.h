#ifndef UNTITLED6_INSTRUCTIONS_H
#define UNTITLED6_INSTRUCTIONS_H
#include <stdint.h>
#include "defines.h"

 #define   MODE_IMMEDIATE 0  /* #5  */
  #define    MODE_DIRECT 1   /* LOOP (label)  */
    #define  MODE_RELATIVE 2   /* &LOOP  */
  #define    MODE_REGISTER 3   /* r0-r7  */

typedef struct
{
    const char name[5]; /* Register name (e.g., "r0")  */
    short value;        /* Corresponding value (e.g., 0 for r0)  */
} RegMap;

typedef struct
{
    char symbol[32]; /* in case its associated with a symbol /TO DO: DO I NEED IT? IF YES SHOULD CHNGE TO A SYMBOL PTR?  */
    int *nums;/* for .data  */
    char *str;/* for .string  */
    int len;
    int isData;/* 1 if its data, 0= string  */

} Directive;

typedef struct {
     char name[6]; /*instruction name ex: add sub ...*/
    int opcode;
     int funct;
    int numOperands; /*1-2*/
   

} InstrucOp;






int isInstruction(const char *name);
int isReg(const char *name);
int isDirective(const char* name);/* returns 4 if its entry, 3 extern, 1 data, 2 string, 0 none */
int getInstructionOp(InstrucOp *out, const char *name);
int addressingOps(Instruction *instruc);/*checkAddressingModes*/
#endif /* UNTITLED6_INSTRUCTIONS_H  */
