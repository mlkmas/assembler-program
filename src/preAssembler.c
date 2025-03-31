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
            //is it an entry who has a macro name?
            if (token && strcmp(token, ".entry") == 0)
            {
                // Check if the token following .entry is a macro name
                char *entryName = strtok(NULL, " \t\n");
                if (entryName && searchMcro(*head, entryName))
                {
                    // Error: .entry references a macro name
                    printf("Error: Line %d: .entry directive references a macro name '%s'\n", lineCounter, entryName);
                    fclose(fp);
                    fclose(finalFile);
                    return 0;
                }
                fprintf(finalFile, "%s %s\n", token, entryName);
            }
            //is it a mcro call?
            else if(token) {
                //first check if this a symbol
                // Check if the token ends with ':'
                size_t len = strlen(token);
                if (len > 0 && token[len - 1] == ':') {
                    // It's a label or symbol, strip the ':' to get the symbol name
                    token[len - 1] = '\0'; // Remove the colon

                    // Check if the symbol name is a macro name
                    if (searchMcro(*head, token)) {
                        // Error: Symbol name matches a macro name
                        printf("Error: Line %d: Symbol '%s' cannot have the same name as a macro\n", lineCounter,
                               token);
                        fclose(fp);
                        fclose(finalFile);
                        //TO DO : I NEED TO CONTINUE LOOKING FOR ERRORS THEN STOP
                        return 0;
                    }

                    // Write the label line to the output file
                    fprintf(finalFile, "%s:\n", token); // Add the colon back for output
                } else {
                    // Check if it's a macro call
                    Node *mcro = searchMcro(*head, token);
                    if (mcro) {
                        // It's a macro call
                        fprintf(finalFile, "%s", mcro->value);
                    } else {
                        // It's a regular line, copy it as-is
                        fprintf(finalFile, "%s", str);
                    }
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