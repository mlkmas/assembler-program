#include "../include/preAssembler.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/instructions.h"
void processFile(const char* inputFilename, const char* outputFilename)
{
    char line[MAX_LINE_LENGTH];
    FILE *inputFile= fopen(inputFilename, "r");
    FILE *outputFile= fopen(outputFilename, "w");
    if(!inputFile||!outputFile)
    {
        //TO DO
        //throw error opening a file
    }
    while (fgets(line,sizeof (line),inputFile))
    {
        line[strcspn(line,"\n")]='\0';
        if(isEmptyLine(line)||isCommentLine(line))
        {
            continue;
        }
        removeWhitespace(line);
        fprintf(outputFile,"%s\n",line);
    }
    fclose(inputFile);
    fclose(outputFile);
}

int preExec(char fileName[])
{
    Node *head=NULL;
    const char newFileName[MAX_FILE_NAME];
    generateOutputFilename(fileName,newFileName,".clean");
    processFile(fileName,newFileName);
    if(newFileName ==NULL)
    {
       //empry file
        return 0;
    }
    if(!handleMcro(newFileName,&head))
    {
        freeList(head);
        //TO DO
        //CLOSE ALL FILES, FREE ALL MEMORY
        return 0;
    }
    return 1;
}
int handleMcro(char *fileName, Node **head)
{
    int lineCounter,status,mcroDef;
    FILE *fp, *finalFile,*outputFileName;
    char str[MAX_LINE_LENGTH], *token, *mcroName, *macroBody;
    fpos_t pos;
    status=1,lineCounter=0,mcroDef=0;
    fp= fopen(fileName,"r");
    if(!fp)
    {
        //throw error
        return 0;
    }
     generateOutputFilename(fileName,outputFileName, ".am");
    finalFile = fopen(outputFileName, "w");
    if (!finalFile)
    {
        //throw error
        fclose(fp);
        return 0;
    }
    while (fgets(str,MAX_LINE_LENGTH,fp))
    {
        lineCounter++;
        token=strtok(str," \t\n");

        //its a definition od a mcro
        if(token && strcmp(token,"mcro")==0)
        {
            mcroDef=1;
            mcroName= strtok(NULL," \t\n");
            if (strtok(NULL, " \t\n") != NULL)
            {
                //error the line has extra letters
                return 0;
            }
            if(!validMcroName(mcroName,*head))
            {
                freeList(*head);
                //TO DO CLOSE ALL FILE+FREE MEMORY
                status=0;
                break;
            }
            else
            {
                macroBody=(char *) malloc(MAX_LINE_LENGTH);
                if(!macroBody)
                {
                    //malloc failed throw error
                    //free+close
                    status=0;
                    break;
                }
                macroBody[0]='\0';
                while (fgets(str,MAX_LINE_LENGTH,fp))
                {
                    lineCounter++;
                    token= strtok(str," \t\n");
                    if(token && strcmp(token, "mcroend")==0)
                    {
                        //TO CHECK IF YHE MCROEND DOESNT HAVE A SEQUENSE LETTERS
                        if (strtok(NULL, " \t\n") != NULL)
                        {
                            //error the line has extra letters
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
        else //its not a mcro definition
        {
            //is it a mcro call?
            if(token)
            {
                Node *mcro=searchMcro(*head, token);
                if(mcro)
                {
                    //its a call
                    fprintf(finalFile, "%s", mcro->value);
                }
                else //its a line copy it as it is
                {
                    fprintf(finalFile, "%s", str);
                }

            }

        }

    }

}
//gets a !NULL list, return the node with the same key, else returns NULL
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
            //error
            return 0;
        }
    }
Node *current=head;

    while (current)
    {
        if(strcmp(current->key,name)==0)
        {
            //error mcro name is declared before
            return 0;
        }
        current=current->next;
    }
if(isInstruction(name))
{
    //error its an isntruction name
    //free+close+break
    return 0;
}
if(isDirective(name) != 0)
{
    //error
    return 0;
}
if(isReg(name))
{
    //error
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
    free(node->key);
    free(node->value);
    free(node);
}