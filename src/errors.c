#include "../include/errors.h"
#include <stdio.h>
#include <stdlib.h>


static const char *errorMessages[ERR_COUNT] = {
        "Could not open file",
        "File write error",
        "File read error",
        "Memory allocation failed",
        "Null pointer dereferenced",
        "Macro syntax error",
        "Invalid argument",
        "Invalid symbol name",
       /* "Invalid symbol format", */
        "Invalid .data format",
         "Duplicate symbol definition",
        "Reserved word used as symbol name",
      
        "Line exceeds 80 characters",
        "Unknown error"
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
