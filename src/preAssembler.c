#include "../include/preAssembler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

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
        freeList(*head);
        //TO DO
        //CLOSE ALL FILES, FREE ALL MEMORY
    }

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
                        if(!validMcroEnd())//TO CHECK IF IT DOESNT HAVE A SEQUENSE LETTERS
                        {
                            //TO DO
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
int validMcroName(Node *head,const char* line)
{
    int i;
    Node *current=head;
    char lineCopy[MAX_LINE_LENGTH];
    char *token;
    const char *name;
    strncpy(lineCopy, line, MAX_LINE_LENGTH);
    lineCopy[MAX_LINE_LENGTH - 1] = '\0';
    token = strtok(lineCopy, " \t\n");
    if (!token || strcmp(token, "mcro") != 0)
    {
        return 0;
    }
    token = strtok(NULL, " \t\n");
    if (!token) {
        return 0;
    }
    name=token;
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
    token = strtok(NULL, " \t\n");
    if(token)
    {
        //error , there are extra letters/tokens
        return 0;
    }

    while (current)
    {
        if(strcmp(current->key,name)==0)
        {
            //error mcro name is declared before
            return 0;
        }
        current=current->next;
    }
/TO DO: CHECK IF THE MACRO NAME IS A RESERVED INSTRUCTION

    return 1;
}