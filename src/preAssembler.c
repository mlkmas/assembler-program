#include "../include/preAssembler.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/instructions.h"
#include "../include/errors.h"
void processFile(const char* inputFilename, const char* outputFilename)
{
    char line[MAX_LINE_LENGTH];
    FILE *inputFile= fopen(inputFilename, "r");
    FILE *outputFile= fopen(outputFilename, "w");
    if(!inputFile||!outputFile)
    {
        handleError(ERR_OPENING_FILE,0,"");
        return;
    }
    while (fgets(line,sizeof (line),inputFile))
    {
        line[strcspn(line,"\r\n")]='\0';
        if(isEmptyLine(line)||isCommentLine(line))
        {
            continue;
        }
        /*removeWhitespace(line);*/
        fprintf(outputFile,"%s\n",line);
    }
    fclose(inputFile);
    fclose(outputFile);
}

int preExec(char fileName[])
{
    Node *head=NULL;
     char newFileName[MAX_FILE_NAME];
    generateOutputFilename(fileName,newFileName,".clean");
    processFile(fileName,newFileName);
    if(newFileName ==NULL ||newFileName[0]=='\0')
    {
        handleError(ERR_MEM_ALLOC,0,"");
        return 0;
    }
    if(!handleMcro(newFileName,&head))
    {
        freeList(head);
        return 0;
    }
    return 1;
}
int handleMcro(char *fileName, Node **head)
{
    int lineCounter,status,mcroDef;
    FILE *fp, *finalFile;
    char outputFileName[MAX_FILE_NAME];
    char str[MAX_LINE_LENGTH], *token, *mcroName, *macroBody;
    fpos_t pos;
    status=1,lineCounter=0,mcroDef=0;
    fp= fopen(fileName,"r");
    if(!fp)
    {

        return 0;
    }
     generateOutputFilename(fileName,outputFileName, ".am");
    finalFile = fopen(outputFileName, "w");
    if (!finalFile)
    {
        fclose(fp);
        return 0;
    }
    while (fgets(str,MAX_LINE_LENGTH,fp))
    {
        char originalLine[MAX_LINE_LENGTH];
        str[strcspn(str, "\r\n")] = '\0';
        lineCounter++;
        strcpy(originalLine, str); 
        token=strtok(str," \t\n");

        /*its a definition od a mcro*/
        if(token && strcmp(token,"mcro")==0)
        {
            mcroDef=1;
            mcroName= strtok(NULL," \t\n");
            if (strtok(NULL, " \t\n") != NULL)
            {
                handleError(ERR_MACRO_SYNTAX, lineCounter, fileName); 
   
                fclose(fp);
                fclose(finalFile);
                return 0;
            }
            if(!validMcroName(mcroName,*head))
            {
                freeList(*head);
                handleError(ERR_MACRO_SYNTAX, lineCounter, fileName);  
                status=0;
                break;
            }
            else
            {
                macroBody=(char *) malloc(MAX_LINE_LENGTH);
                if(!macroBody)
                {
                    handleError(ERR_MEM_ALLOC, lineCounter, fileName);
                    status=0;
                    break;
                }
                macroBody[0]='\0';
                while (fgets(str,MAX_LINE_LENGTH,fp))
                {
                    str[strcspn(str, "\r\n")] = '\0';
                    lineCounter++;
                    token= strtok(str," \t\n");
                    if(token && strcmp(token, "mcroend")==0)
                    {
                        /*TO CHECK IF YHE MCROEND DOESNT HAVE A SEQUENSE LETTERS*/
                        if (strtok(NULL, " \t\n") != NULL)
                        {
                            handleError(ERR_MACRO_SYNTAX,lineCounter,fileName);
                            return 0;
                        }
                        mcroDef=1;
                        if(!addMcro(head,mcroName,macroBody))
                        {
                            free(macroBody);
                            status=0;
                            break;
                        }
                        free(macroBody);
                        break;
                    }
                    else
                    {
                        strcat(macroBody,str);
                    }
                }

            }
        }
        else /*its not a mcro definition*/
        {
            /*is it an entry who has a macro name? */
            if (token && strcmp(token, ".entry") == 0)
            {
                /*Check if the token following .entry is a macro name*/
                char *entryName = strtok(NULL, " \t\n");
                if (entryName && searchMcro(*head, entryName))
                {
                    handleError(ERR_INVALID_SYM_NAME,lineCounter,fileName);
                    fclose(fp);
                    fclose(finalFile);
                    return 0;
                }
               fprintf(finalFile, "%s\n", originalLine);
            }
            /*is it a mcro call? */
            else if(token) {
                /*first check if this a symbol
                 Check if the token ends with ':' */
                size_t len = strlen(token);
                if (len > 0 && token[len - 1] == ':') {
                    /*It's a label or symbol, strip the ':' to get the symbol name */
                    token[len-1]='\0'; /*Remove the colon */

                    /*Check if the symbol name is a macro name */
                    if (searchMcro(*head, token))
                     {
                        /*Error: Symbol name matches a macro name*/
                        handleError(ERR_DUPLICATE_SYMBOL,lineCounter, fileName);
                        fclose(fp);
                        fclose(finalFile);
                        /*TO DO : I NEED TO CONTINUE LOOKING FOR ERRORS THEN STOP*/
                        return 0;
                    }

                    /*Write the label line to the output file*/
                    fprintf(finalFile, "%s\n", originalLine); /*Add the colon back for output*/
                } else {
                    /*Check if it's a macro call */
                    Node *mcro = searchMcro(*head, token);
                    if (mcro)
                     {
                        /*It's a macro call*/
                        fprintf(finalFile, "%s\n", mcro->value);
                    } else {
                        /*It's a regular line, copy it as-is */
                       fprintf(finalFile, "%s\n", originalLine);
                    }
                }
            }

        }

    }

fclose(fp);
    fclose(finalFile);
    return status;
}
/* gets a !NULL list, return the node with the same key, else returns NULL  */
Node *searchMcro(Node *head, const char *name)
{
    while(head!=NULL)
    {
        if(strcmp(head->key,name)==0)
            return head;
        head=head->next;
    }
    return NULL;
}
int validMcroName(const char* name,Node *head)
{
 int i;
    if (!name || !isalpha(name[0]))
    {
        return 0;
    }
    for( i=0;name[i];i++)
    {
        if(!isalnum(name[i]))
        {
            /* error  */
            return 0;
        }
    }
Node *current=head;

    while (current)
    {
        if(strcmp(current->key,name)==0)
        {
            /* error mcro name is declared before  */
            return 0;
        }
        current=current->next;
    }
if(isInstruction(name))
{
    /* error its an isntruction name  */
    /* free+close+break  */
    return 0;
}
if(isDirective(name) != 0)
{
    /* error  */
    return 0;
}
if(isReg(name))
{
    /* error  */
    return 0;
}

    return 1;
}

void freeList(Node *head)
{
    while(head != NULL) {
        Node *tmp = head;
        head = head->next;
        freeNode(tmp);
    }
}
void freeNode(Node *node)
{
    
    free(node);
}
int addMcro(Node **head, const char *name, const char *body)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(!newNode)
    {
        return 0;
    }
    strncpy(newNode->key, name, MAX_LABEL_LENGTH-1);
    newNode->key[MAX_LABEL_LENGTH-1] = '\0';
    strncpy(newNode->value, body, MAX_VALUE_LEN-1);
    newNode->value[MAX_VALUE_LEN-1] = '\0';
    newNode->next = *head;   /* insert at front of list */
    *head = newNode;
    return 1;
}