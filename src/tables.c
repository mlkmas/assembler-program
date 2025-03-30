//
// Created by malak on 30/03/2025.
//
#include <ctype.h>
#include <string.h>
#include "../include/tables.h"
#include "../include/instructions.h"
#include "../include/preAssembler.h"
void insertSymbol(Symbol symbolTable[], char *name, int value, int isExtern,int isEntry, int type, int symbolCount) // ones digit is for entr, tens for ectern
{
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].value = value;
    symbolTable[symbolCount].isEntry = isEntry ;
    symbolTable[symbolCount].isExternal = isExtern;
    symbolTable[symbolCount].type = type;
    symbolCount++;
}

int validSymbol(Symbol *symbol, Symbol symbolTable[], int symbolCount)
{
    int i;
    if (!symbol->name || !isalpha(symbol->name[0]))
    {
        return 0;
    }
    for( i=0;symbol->name[i];i++)
    {
        if(!isalnum(symbol->name[i]))
        {
            //error
            return 0;
        }
    }
    for (i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbolTable[i].name, symbol->name) == 0)
        {
            //TO DO PRINT error
            return 0; // Symbol already exists
        }
    }
    //TO DO IS MACRO CHECKS IF THE SYMBOL IS AN EXISTING MACRO NAME+ EDIT THE PREASSEMBLER TO SAVE MACRO NAMES
    if(isInstruction(symbol->name)==1 || isMacro(symbol->name)==1)
    {
        //its not a valis name TO DO ERROR
        return 0;
    }
    return 1; // Valid symbol
}