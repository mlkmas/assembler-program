#ifndef UNTITLED6_ERRORS_H
#define UNTITLED6_ERRORS_H


typedef enum {
    ERR_OPENING_FILE,
    ERR_WRITING_,
    ERR_FILE_READ,
    ERR_MEM_ALLOC,
    ERR_NULL_POINTER,
    ERR_MACRO_SYNTAX,
    ERR_INVALID_ARGUMENT,
    ERR_INVALID_SYM_NAME,
    ERR_DUPLICATE_SYMBOL,
    ERR_INVALID_INSTRUCTION_NAME,
    ERR_INVALID_SYM_FORMAT,
    ERR_INVALID_DATA_FORMAT,
    ERR_UNKNOWN
} ErrorCode;

/*
 * Handles an error based on error code .
 * Does NOT exit the program, format's error.
 */
void handleError(ErrorCode code);

/*
 * Handles a system error and exits the program.
 */
void sysError(ErrorCode code);


#endif //UNTITLED6_ERRORS_H
