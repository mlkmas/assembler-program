#ifndef UNTITLED6_FIRSTPASS_H
#define UNTITLED6_FIRSTPASS_H
#include "instructions.h"
struct commands{

};
typedef struct {
    char name[MAX_LINE_LENGTH];  //max label length
    int value;                   // Address value
    unsigned is_entry : 1;       // Uses 1 bit (0 or 1)
    unsigned is_external : 1;    // Uses 1 bit (0 or 1)
    unsigned type : 2;           // 2 bits for 4 possible types (00=none, 01=data, 10=string, 11=code)
} Symbol;



int firstPartExe(char *fileNmes);

#endif //UNTITLED6_FIRSTPASS_H
