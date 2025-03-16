#ifndef UNTITLED6_PREASSEMBLER_H
#define UNTITLED6_PREASSEMBLER_H
#include "assembler.h"

int preExec(char file[]);
/***
*
* @param inputFilename
* @param outputFilename
*/
void processFile(const char* inputFilename, const char* outputFilename);
int handleMcro(char *fileName, Node **head);
int validMcroName(const char *name,Node **head);
int addMcro(Node **head, const char *name, const char *body);
Node *searchMcro(Node *head, const char *name);
char *replaceMcro(Node *head, const char *line);
void freeList(Node *head);


#endif //UNTITLED6_PREASSEMBLER_H
