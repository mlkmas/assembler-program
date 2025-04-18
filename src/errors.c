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
        "Invalid argument passed",
        "Unknown error"
};

void handleError(ErrorCode code)
{
    if (code < 0 || code >= ERR_COUNT) code = ERR_UNKNOWN;
    fprintf(stderr, "[ERROR] %s", errorMessages[code]);
    fprintf(stderr, "\n");
}

void sysError(ErrorCode code)
{
    handleError(code);
    exit(EXIT_FAILURE);
}
