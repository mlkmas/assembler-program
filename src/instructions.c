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
        {NULL, -1}
};
const Instruc instructions[] = {
        {"add",  2, 2, "01"},
        {"sub",  2, 2, "01"},
        {"mov",  0, 2, "01"},
        {"cmp",  1, 2, "01"},
        {"lea",  4, 2, "01"},
        {"clr",  5, 1, "01"},
        {"not",  5, 1, "01"},
        {"inc",  5, 1, "01"},
        {"dec",  5, 1, "01"},
        {"jmp",  9, 1, "01"},
        {"bne", 9, 1, "01"},
        {"jsr", 9, 1, "01"},
        {"red", 12, 1, "01"},
        {"prn", 13, 1, "01"},
        {"rts", 14, 0, "01"},
        {"stop",15, 0, "01"},
        {NULL,  -1, 0, NULL}
};

char *Symbols[]={".data", ".string", ".extern", ".entry"};
int isInstructuion(char *name)
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
int isSymbol(char* name)
{
    if(name ==NULL)
        return 0;
    int i;
    for(i=0;i<4;i++)
    {
        if(strcmp(name,Symbols[i])==0)
            return 1;
    }
    return 0;
}