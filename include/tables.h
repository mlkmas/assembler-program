#ifndef UNTITLED6_TABLES_H
#define UNTITLED6_TABLES_H
#include "instructions.h"
#include <stddef.h>

typedef struct
{
    char name[MAX_LABEL_LENGTH+1];
    int value;            /* Address (IC or DC)  */
    int lineNum;         /* Source line for error reporting  */
    unsigned isEntry : 1; /* 1 entry 0 not  */
    unsigned isExternal : 1; /* 1=extern  */
    unsigned isData : 1; /* 1=.data/.string, 0=instruction  */
} Symbol;

typedef struct
{
    char *fileName; /* file name  */
    int lineNum;/* line nums error  */
} SourcePosition; /* for tracking lines and its file  */


/* Structure to represent other tables ( externs, entries) */
typedef struct
{
    char name[MAX_LABEL_LENGTH+1];
    int line;

}extEntTable;

/*Parses a label definition from a source line and fills the Symbol struct with its name and type.
 * Returns: void. */
void extractSymbol(char *line, Symbol *symbol,int isData);

/* Checks that a symbol is not a duplicate or reserved word in the symbol table.
 * Returns: 1 if valid, 0 if invalid.
  */
int validSymbol(Symbol *symbol, Symbol symbolTable[], int symbolCount);

/* Inserts a new symbol into the symbol table, resizing the table if needed and adjusting its address by DC.
 * Returns:void
  */
void insertSymbol(Symbol **symbolTable, Symbol *symbol, int *symbolCount,size_t *symbolSize,int DC);

/* Reallocates a generic table pointer to the given new size.
 * Returns: 1 on success, 0 on allocation failure.
  */
int resizeTable(void **table, size_t newSize, size_t elemCounter);

/* Dispatches parsing of a .data or .string directive line based on the directive type.
 * Returns: void
  */
void processDataOrStr(int res,Directive *directiveInst,char *line,int *err);

/* Parses comma-separated integers from a .data line into the directive's operand array.
 * Returns: the count of numbers parsed, or -1 on error
  */
int extractNums(char *lineCopy, Directive *dir,int *err);

/* Counts the number of comma-separated numeric tokens in a .data line.
 * Returns: the integer count of numbers found
 */
int countNums(const char *line);

/* Extracts the string operand from a .string directive line into the directive struct.
 * Returns: 0 on success, -1 on error
 */
int extractStr(char *lineCopy, Directive *dir,int *err);

/* Appends a completed Directive to the directives array, resizing the array if needed.
 * Returns: void
 */
void insertDir(Directive *directiveInst,Directive **directives, int *err, size_t *dirCapacity,int *dirCount);

/* Converts a string token to an integer, validating that it is a proper number.
 * Returns: 1 on success, 0 if the string is not a valid integer
 */
int extractNum(int *num, const char *str);

/* Parses a full instruction line into an Instruction struct, resolving addressing modes and operands.
 * Returns: the number of machine words the instruction occupies, or -1 on error
 */
int parseInstruction(char *line, Instruction *instruc, int IC, int *err, int symbolFlag);

/* Appends an Instruction to the instruction array, resizing the array if needed.
 * Returns: 1 on success, 0 on allocation failure
 */
int insertInstruction(Instruction *instruction,Instruction **instrucs,size_t *instCapactiy,int *intrucsCounter);

/* Encodes the first machine word of an instruction based on opcode and addressing modes.
 * Returns: void
 */
void buildFirstWord(Instruction *ins,int *err);

/* Fills the extra machine word for a label operand with its resolved address and ARE bits.
 * Returns: void
 */
void buildLabelMW(Instruction *ins,int add,int i);

/* Allocates and encodes machine words for a .data or .string directive.
 * Returns:void
 */
void setDataMWord(MachineWord **mw, int *wordsCount, int *err, Directive *dir);

/* Adds the final instruction counter value (ICF) to the address of every data/string symbol.
 * Returns: void
 */
void updateDataSymbols(Symbol *SymbolTable, int symbolCount,int ICF);

/* Encodes the extra machine words (immediate values, register pairs) for an instruction's operands.
 * Returns: void
 */
void buildExtraWords(Instruction *ins);



#endif /* UNTITLED6_TABLES_H  */
