#ifndef UNTITLED6_PREASSEMBLER_H
#define UNTITLED6_PREASSEMBLER_H

#include "utils.h"

typedef struct Node
{
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LEN];
    struct Node* next;
}Node;

void freeList(Node *head);
void freeNode(Node *node);

int preExec(char file[]);
/***
*
* @param inputFilename
* @param outputFilename
*/
void processFile(const char* inputFilename, const char* outputFilename);
int handleMcro(char *fileName, Node **head);
int validMcroName(const char *name, Node *head);
int addMcro(Node **head, const char *name, const char *body);
Node *searchMcro(Node *head, const char *name);

void freeList(Node *head);



#endif //UNTITLED6_PREASSEMBLER_H
