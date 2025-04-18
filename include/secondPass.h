#ifndef UNTITLED6_SECONDPASS_H
#define UNTITLED6_SECONDPASS_H
#include "../include/tables.h"


/**
 * @brief Executes the second pass of the assembler
 *
 * Processes the intermediate representation from first pass to:
 * 1. Generate machine code words
 * 2. Validate symbol references
 * 3. Create output files (.ob, .ent, .ext)
 *
 * @param file_name Input assembly file name
 * @param symbolTable Symbol table from first pass
 * @param IC Instruction counter value
 * @param DC Data counter value
 * @param symbolCount Number of symbols in table
 * @param externsCounter Number of extern symbols
 * @param entriesCounter Number of entry symbols
 * @param dataWords Data words from first pass
 * @param externs Extern table
 * @param entries Entry table
 * @param err Error flag
 * @param instrcs Instructions array
 * @param instrcsCount Instruction count
 * @param L  length
 * @return int 1 on success, 0 on failure
 */
int secondPartExec(char *file_name, Symbol *symbolTable , int IC, int DC, int symbolCount, int externsCounter,
                   int entriesCounter, MachineWord *dataWords, extEntTable *externs, extEntTable *entries, int err, Instruction *instrcs, int instrcsCount, int L);

int writeMWordsToHexObFile(const char *filename, MachineWord *codeWords, int Codelen,MachineWord *dataWords,int dataLen);

int checkSymbolTables(Symbol *symbolTable,extEntTable *externs,extEntTable *entries,int entriesCounter,int externsCounter,int symbolCount,int *err);
int searchSymbolInotherTables(Symbol *symbol,extEntTable *table,int count);
int searchSymByName(Symbol *symTable,int symCount,char *name);
int setInstLabelsMw(Symbol *symTable,int symCount,int *err,Instruction *instrucs,int insCount,MachineWord **externWords,int *externsCount,size_t *externCap);
void buildExternMW(MachineWord **externWords,int add,int i);
int insertExternWord(int *externsCount,size_t *externCap,MachineWord **externWords,int *err,int address,char name[32]);
MachineWord* createCodeWords(Instruction *instrcs, int instrcsCount, int totalWords);
int printExtEntTable(MachineWord *words,int len, char *extension,char *fileName);
MachineWord* createEntWords(extEntTable *ents, int totalWords);

#endif //UNTITLED6_SECONDPASS_H
