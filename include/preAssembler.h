#ifndef UNTITLED6_PREASSEMBLER_H
#define UNTITLED6_PREASSEMBLER_H

#include "utils.h"

/*
 * Linked list node to hold macro definitions.
 * 'key' stores the macro name, 'value' stores the macro body.
 */
typedef struct Node
{
    char key[MAX_LABEL_LENGTH];
    char value[MAX_VALUE_LEN];
    struct Node* next;
}Node;

/*
 * Frees the entire linked list starting from head.
 */
void freeList(Node *head);

/*
 * Frees a single node.
 */
void freeNode(Node *node);

/*
 * Executes the pre-assembler process on the given file.
 * @param file - input file name with .as extension
 * @return 1 if success, 0 if failure
 */
int preExec(char file[]);

/*
* Cleans whitespace and comments from input file and writes to output file.
* @param inputFilename - original input file name
* @param outputFilename - cleaned output file name
*/
void processFile(const char* inputFilename, const char* outputFilename);

/*
 * Processes macro definitions and replaces them in file.
 * @param fileName - name of the file to read
 * @param head - pointer to the macro list head
 * @return 1 if success, 0 if failure
 */
int handleMcro(char *fileName, Node **head);

/*
 * Checks if macro name is valid (not instruction, directive, or register).
 * @param name - macro name to validate
 * @param head - macro list head
 * @return 1 if valid, 0 if invalid
 */
int validMcroName(const char *name, Node *head);

/*
 * Adds a macro to the list.
 * @param head - pointer to macro list
 * @param name - macro name
 * @param body - macro content
 * @return 1 if success, 0 if failure
 */
int addMcro(Node **head, const char *name, const char *body);

/*
 * Searches for a mcro in the list by name.
 * @param head - list head
 * @param name - macro name to search
 * @return pointer to Node if found, else NULL
 */
Node *searchMcro(Node *head, const char *name);


#endif //UNTITLED6_PREASSEMBLER_H
