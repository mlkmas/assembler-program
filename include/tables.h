#ifndef UNTITLED6_TABLES_H
#define UNTITLED6_TABLES_H
#include "defines.h"

typedef struct
{
    char name[MAX_LINE_LENGTH];  //max label length
    int value;                   // Address value
    int isEntry : 1;       // Uses 1 bit (0 or 1)
    int isExternal : 1;    // Uses 1 bit (0 or 1)
    int type : 2;           // 2 bits for 4 possible types (00=none, 01=data, 10=string, 11=code)
} Symbol;


void insertSymbol(Symbol **symbolTable, char *name, int value, int isExtern,int isEntry, int type, int *symbolCount);





#endif //UNTITLED6_TABLES_H
