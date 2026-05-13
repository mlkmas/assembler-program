/**
 * firstpass.h
 * builds the sumbol table encodes instruction&directives
 * and hands results to the second pass
 * 
 */


#ifndef UNTITLED6_FIRSTPASS_H
#define UNTITLED6_FIRSTPASS_H

#include "../include/tables.h"
#include <stddef.h>   

/**
 * Executes the first pass with the .am file.
 * Builds the symbol table, the instruction list, the data section,
 * and the externs/entries tables. On success, hands them to the
 * second pass.
 *
 * @param fileName  Path to the .am file (after macro expansion).
 * @return 1 on success, 0 if any error was detected.
 */
int firstPartExe(char *fileName);

/**
 * Inserts a name into the externs OR entries table, and resize size when needed
 *
 * @param table     The table to insert into.
 * @param count     In/out: current number of entries.
 * @param name      Symbol name to insert.
 * @param capacity  In/out: current allocated capacity.
 * @param line      Source line number for error reporting.
 * @return 1 on success, 0 on allocation failure.
 */
int inserExternEntry(extEntTable *table,int *count,char name[MAX_LABEL_LENGTH+1],size_t *capacity,int line);

#endif /* UNTITLED6_FIRSTPASS_H  */
