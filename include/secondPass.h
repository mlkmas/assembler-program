#ifndef UNTITLED6_SECONDPASS_H
#define UNTITLED6_SECONDPASS_H
#include "../include/tables.h"

int secondPartExec(char *file_name,  Symbol *symbolTable , int IC, int DC, int symbolCount,int externsCounter,
                   int entriesCounter, MachineWord *dataMWs, extEntTable *externs, extEntTable *entries, int err,Instruction *instrcs, int instrcsCount);

int writeMWordsToHexObFile(const char *filename, MachineWord *words, int len);

int checkSymbolTables(Symbol *symbolTable,extEntTable *externs,extEntTable *entries,int entriesCounter,int externsCounter,int symbolCount,int *err);
int searchSymbolInotherTables(Symbol *symbol,extEntTable *table,int count)
#endif //UNTITLED6_SECONDPASS_H
