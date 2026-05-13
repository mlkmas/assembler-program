/**
 * @file secondPass.h
 * @brief handles the label ref to extra words and generates entry&extern tables
 * and writes ob,ent and ext file
 * @version 0.1
 * @date 2026-05-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#ifndef UNTITLED6_SECONDPASS_H
#define UNTITLED6_SECONDPASS_H
#include "../include/tables.h"
#include <stddef.h>   

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

/**
 * @brief write the ob file 
 * 
 * @param filename 
 * @param codeWords 
 * @param Codelen 
 * @param dataWords 
 * @param dataLen 
 * @return 1 success ,0 otherwise
 */
int writeMWordsToHexObFile(const char *filename, MachineWord *codeWords, int Codelen,MachineWord *dataWords,int dataLen);
/**
 * @brief checks the symbol table for entry and extern symbols in terms
 * of correct defination
 * 
 * @param symbolTable 
 * @param externs 
 * @param entries 
 * @param entriesCounter 
 * @param externsCounter 
 * @param symbolCount 
 * @param err 
 * @return int 1 if no errors
 */
int checkSymbolTables(Symbol *symbolTable,extEntTable *externs,extEntTable *entries,int entriesCounter,int externsCounter,int symbolCount,int *err);
/**
 * @brief Returns 1 if 'symbol' appears in 'table', else 0
 * 
 * @param symbol 
 * @param table 
 * @param count 
 * @return int 
 */
int searchSymbolInotherTables(Symbol *symbol,extEntTable *table,int count);
/**
 * Linear search for a symbol by name.
 *
 * @return index in symTable, or -1 if not found
 */
int searchSymByName(Symbol *symTable,int symCount,char *name);

/*
* Resolves each label operand in the instruction list. For
 * external references, records the patch address into the
 * externWords table.
 *
 * @return 1 on success, 0 on allocation failure.
 * */
int setInstLabelsMw(Symbol *symTable,int symCount,int *err,Instruction *instrucs,int insCount,MachineWord **externWords,int *externsCount,size_t *externCap);
/*
builds an extern machine word 
*/
void buildExternMW(MachineWord **externWords,int add,int i);
/*
records external reference address and resize when needed
*/
int insertExternWord(int *externsCount,size_t *externCap,MachineWord **externWords,int *err,int address,char name[MAX_LABEL_LENGTH+1]);
/**
 * @brief Create a Code Words object
 * 
 * @param instrcs 
 * @param instrcsCount 
 * @param totalWords 
 * @return MachineWord* 
 */
MachineWord* createCodeWords(Instruction *instrcs, int instrcsCount, int totalWords);
/*
writes the ent and ext files 
return 1 on sucees.0 otherwise
*/
int printExtEntTable(MachineWord *words,int len, char *extension,char *fileName);
/* builds the entries mw array
returns an array, or NULL in case of failure*/
MachineWord* createEntWords(extEntTable *ents, int totalWords,Symbol *symTable, int symCount);

#endif /* UNTITLED6_SECONDPASS_H  */
