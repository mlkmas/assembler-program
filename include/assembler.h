#ifndef UNTITLED6_ASSEMBLER_H
#define UNTITLED6_ASSEMBLER_H
#include "utils.h"
typedef struct Node
{
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LEN];
    struct Node* next;
}Node;


#endif
