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
const InstrucOp instructions[] =
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
        return 0;
    int i;
    for(i=0;i<9;i++)
    {
        if(strcmp(name,regTable[i].name)==0)
            return 1;
    }
    return 0;
}
int isDirective(char* name)
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