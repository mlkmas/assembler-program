#include "../include/instructions.h"
#include <string.h>


// Define the register names and their corresponding values

const RegMap regTable[] = {
        {"r0", 0},
        {"r1", 1},
        {"r2", 2},
        {"r3", 3},
        {"r4", 4},
        {"r5", 5},
        {"r6", 6},
        {"r7", 7},
        {"psw", 8},
        {"\0", -1}
};
 InstrucOp instructions[] =
        {
        {"add",  2, 2},
        {"sub",  2, 2},
        {"mov",  0, 2},
        {"cmp",  1, 2},
        {"lea",  4, 2},
        {"clr",  5, 1},
        {"not",  5, 1},
        {"inc",  5, 1},
        {"dec",  5, 1},
        {"jmp",  9, 1},
        {"bne",  9, 1},
        {"jsr",  9, 1},
        {"red", 12, 1},
        {"prn", 13, 1},
        {"rts", 14, 0},
        {"stop",15, 0},
        {NULL,  -1, 0}  // Sentinel value to mark the end of the table
};
const char directive[]={".data", ".string", ".extern", ".entry"};
int isInstruction(char *name)
{
    if(name ==NULL)
        return 0;
    int i;
    for(i=0;i<16;i++)
    {
        if(strcmp(name,instructions[i].name)==0)
            return 1;
    }
    return 0;

}

int isReg(char *name)
{
    if(name ==NULL)
        return -1;
    int i;
    for(i=0;i<10;i++)
    {
        if(strcmp(name,regTable[i].name)==0)
            return regTable[i].value;
    }
    return -1;
}
int isDirective(const char* name)
{
    if(name ==NULL)
        return 0;
    int i;
    for(i=0;i<4;i++)
    {
        if(strcmp(name,directive[i])==0)
            return i+1;
    }
    return 0;
}
int setInstOp(Instruction *instruc, const char *token)
{
    int i;
    // Search through the instructions table
    for ( i = 0; instructions[i].name != NULL; i++)
    {
        if (strcmp(instructions[i].name, token) == 0)
        {
            // Found a match - store the pointer
            instruc->inst = &instructions[i];
            return 1;  // Success
        }
    }
    return 0;  // Not found
}

int addressingOps(Instruction *instruc)
{
    int i,op,err;
    err=1;//no error
    op=instruc->inst->opcode;
    /*source reg addressing modes*/

    if(op>=0 && op<2)
    {
        if(instruc->operands[0].mode==2)
        {
            err= 0;
        }
    }
    if(op==4)
    {
        if(instruc->operands[0].mode!=1)
        {
            err= 0;
        }
    }

    /*dist reg addressing modes*/

    if(op==0 ||op>=2 || op==4)
    {
        if(instruc->operands[1].mode==2 || instruc->operands[1].mode==0  )
        {
            err= 0;
        }
    }
    if(op==1)
    {
        if(instruc->operands[1].mode==2 )
        {
            err= 0;
        }

    }

    if(op==5 || op==12)
    {
        if(instruc->operands[0].mode==2 || instruc->operands[0].mode==0  )
        {
            err= 0;
        }
    }
    if(op==13)
    {
        if(instruc->operands[1].mode==2 )
        {
            err= 0;
        }

    }
    if(op==9)
    {
        if(instruc->operands[0].mode==0 || instruc->operands[0].mode==3 )
        {
            err= 0;
        }
    }
    return err;
}