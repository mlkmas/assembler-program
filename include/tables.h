#ifndef UNTITLED6_TABLES_H
#define UNTITLED6_TABLES_H
#include "instructions.h"

typedef struct
{
    char name[32];
    int value;            // Address (IC or DC)
    int lineNum;         // Source line for error reporting
    unsigned isEntry : 1; //1 entry 0 not
    unsigned isExternal : 1; //1=extern
    unsigned isData : 1; // 1=.data/.string, 0=instruction
} Symbol;

typedef struct
{
    char *fileName; //file name
    int lineNum;//line nums error
} SourcePosition; //for tracking lines and its file
typedef struct
{
    char name[32];
    int line;
}extEntTable;
void extractSymbol(char *line, Symbol *symbol,int isData);//TO DO
int validSymbol(Symbol *symbol, Symbol symbolTable[], int symbolCount);
void insertSymbol(Symbol **symbolTable, Symbol *symbol, int *symbolCount,size_t *symbolSize,int DC);
int resizeTable(void **table, size_t newSize, size_t elemCounter);
void processDataOrStr(int res,Directive *directiveInst,char *line,int *err);
int extractNums(char *lineCopy, Directive *dir,int *err);
int countNums(const char *line);
int extractStr(char *lineCopy, Directive *dir,int *err);
void insertDir(Directive *directiveInst,Directive **directives, int *err, size_t *dirCapacity,int *dirCount);
int extractNum(int *num, const char *str);
int parseInstruction(char *line, Instruction *instruc, int IC, int *err, int symbolFlag);
int insertInstruction(Instruction *instruction,Instruction **instrucs,size_t *instCapactiy,int *intrucsCounter);
void buildFirstWord(Instruction *ins,int *err);
void buildLabelMW(Instruction *ins,int *err,int add,int i);
void setDataMWord(MachineWord **mw, int *wordsCount, int *err, Directive *dir);
#endif //UNTITLED6_TABLES_H
