#include "../include/errors.h"
#include <stdio.h>
#include <stdlib.h>


static const char *errorMessages[ERR_COUNT]= 
{
    "Could not open file",                     /* ERR_OPENING_FILE */
    "File write error",                        /* ERR_WRITING_ */
    "File read error",                         /* ERR_FILE_READ */
    "Memory allocation failed",                /* ERR_MEM_ALLOC */
    "Null pointer dereferenced",               /* ERR_NULL_POINTER */
    "Macro syntax error",                      /* ERR_MACRO_SYNTAX */
    "Invalid argument",                        /* ERR_INVALID_ARGUMENT */
    "Invalid symbol name",                     /* ERR_INVALID_SYM_NAME */
    "Duplicate symbol definition",             /* ERR_DUPLICATE_SYMBOL */
    "Reserved word used as symbol name",       /* ERR_INVALID_INSTRUCTION_NAME */
    "Invalid symbol format",                   /* ERR_INVALID_SYM_FORMAT */
    "Invalid .data format",                    /* ERR_INVALID_DATA_FORMAT */
    "Unknown error"                            /* ERR_UNKNOWN */
};

void handleError(ErrorCode code,int lineNum, const char *fileName)
{
    if (code < 0 || code >= ERR_COUNT) code = ERR_UNKNOWN;
    if(lineNum>0) fprintf(stderr, "Error in %s, line %d: %s\n", fileName, lineNum, errorMessages[code]);
      else
        fprintf(stderr, "[ERROR] %s", errorMessages[code]);
    fprintf(stderr, "\n");
}

void sysError(ErrorCode code)
{
    handleError(code,0,"");
    exit(EXIT_FAILURE);
}
