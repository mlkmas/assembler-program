//
// Created by malak on 30/03/2025.
//
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/tables.h"

void insertSymbol(Symbol **symbolTable, Symbol *symbol, int *symbolCount,size_t *symbolSize,int DC) // ones digit is for entr, tens for ectern
{
    size_t capacity=(*symbolSize);
    if ((size_t) symbolCount >= capacity)
    {
        capacity*= 2;  // Double capacity (common strategy)
        if (!resizeTable((void **) &symbolTable, capacity, sizeof(Symbol)))
        {
           //TO DO ERROR RESIZING
            return;
        }
        (*symbolSize)=capacity;
    }
    strcpy(symbolTable[*symbolCount]->name, symbol->name);
    (*symbolTable)[*symbolCount].value = symbol->value;
    (*symbolTable)[*symbolCount].isEntry = symbol->isEntry ;
    (*symbolTable)[*symbolCount].isExternal = symbol->isExternal;
    (*symbolTable)[*symbolCount].isData = symbol->isData;
    (*symbolCount)++;
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

    if(isInstruction(symbol->name)==1 )
    {
        //its not a valis name TO DO ERROR
        return 0;
    }
    return 1; // Valid symbol
}
void extractSymbol(char *line, Symbol *symbol, int isData)
{
    // Initialize symbol defaults
    symbol->name = NULL;
    symbol->isExternal = 0;
    symbol->isData = (isData == 1 || isData == 2) ? 1 : 0;

    char *colonPos = strchr(line, ':');
    char *dirPos = strchr(line, '.');

    // Case 1: Handle labels (with ':')
    if (colonPos != NULL) {
        // Skip leading whitespace (replace while loop)
        char *labelStart = line + strspn(line, " \t");

        // Label ends at colon; trim trailing whitespace
        char *labelEnd = colonPos;
        while (labelEnd > labelStart && isspace(*(labelEnd - 1)))
            labelEnd--;

        int nameLen = labelEnd - labelStart;
        if (nameLen <= 0)
        {
           //TO DO ERROR THERE IS NO LABEL NAME
            return;
        }

        symbol->name = malloc(nameLen + 1);
        if (!symbol->name)
        {
          //ERROR MALLOC
            return;
        }
        strncpy(symbol->name, labelStart, nameLen);
        symbol->name[nameLen] = '\0';
    }
        // Case 2: Handle .extern (no space after '.')
    else if (isData == 3) {
        symbol->isExternal = 1;

        // Skip ".extern" (dirPos points to '.')
        char *labelStart = dirPos + 1;  // Skip '.'
        labelStart += strspn(labelStart, "abcdefghijklmnopqrstuvwxyz"); // Skip "extern"

        // Skip any whitespace after ".extern"
        labelStart += strspn(labelStart, " \t");

        // Extract label (ends at whitespace or newline)
        char *labelEnd = labelStart + strcspn(labelStart, " \t\n");
        int nameLen = labelEnd - labelStart;

        if (nameLen <= 0)
        {
           //TO DO ERROR
            return;
        }

        symbol->name = malloc(nameLen + 1);
        if (!symbol->name)
        {
            //TO DO MALLOC FAILED ERROR
            return;
        }
        strncpy(symbol->name, labelStart, nameLen);
        symbol->name[nameLen] = '\0';
    }
}
int resizeTable(void **table, size_t newSize, size_t elemCounter)
{
    void *newTable = realloc(*table, newSize * elemCounter);
    if (!newTable && newSize > 0) {
        return 0;  // Resize failed
    }
    *table = newTable;
    return 1;
}