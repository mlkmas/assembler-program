#include "../include/instructions.h"
#include <string.h>


/* Define the register names and their corresponding values  */

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
static const InstrucOp instructions[] =
        {
    /*  name    opcode  funct  numOperands */
        {"add",  2,10, 2},
        {"sub",  2,11, 2},
        {"mov",  0,0, 2},
        {"cmp",  1,0, 2},
        {"lea",  4,0, 2},
        {"clr",  5,10, 1},
        {"not",  5,11, 1},
        {"inc",  5,12, 1},
        {"dec",  5,13, 1},
        {"jmp",  9,10, 1},
        {"bne",  9,11, 1},
        {"jsr",  9,12, 1},
        {"red", 12,0, 1},
        {"prn", 13,0, 1},
        {"rts", 14,0, 0},
        {"stop",15,0, 0},
        {"\0",  -1, 0,0}  /*sentinel*/
};
static const char *directive[]={".data", ".string", ".extern", ".entry",NULL};
int isInstruction(const char *name)
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

int isReg(const char *name)
{
    if(name ==NULL)
        return -1;
    int i;
    for(i=0;i<7;i++)
    {
        if(strcmp(name,regTable[i].name)==0)
            return regTable[i].value;
    }
    return -1;
}
int isDirective(const char* name)
{
    int i;
    if(!name)
        return 0;
    
    for(i=0;directive[i]!=NULL;i++)
    {
        if(strcmp(name,directive[i])==0)
            return i+1;
    }
    return 0;
}
int getInstructionOp(InstrucOp *out, const char *name)
{
    int i;
    /*Search through the instructions table*/
    for ( i = 0; instructions[i].opcode != -1; i++)
    {
        if (strcmp(instructions[i].name, name) == 0)
        {
            
            *out= instructions[i];
            return 1; 
        }
    }
    return 0;  /*not found */
}
/*checkAddressingModes*/
int addressingOps(Instruction *instruc)
{
    int op;
    op=instruc->opcode;
   
    if(op==4)
    {
        if(instruc->mode[0]!=1)
        {
            return 0;
        }
    }

    /*dist reg addressing modes*/

    if(op==0 ||op==2 || op==4)
    {
        if(instruc->mode[1]==0  )
        {
            return 0;
        }
    }
    if(op==12 || op==5)
    {
        if(instruc->mode[0]==0 )
        {
            return 0;
        }

    }

    
    if(op==9)
    {
        if(instruc->mode[0]==0 || instruc->mode[0]==3 )
        {
           return 0;
        }
    }
    return 1;
}