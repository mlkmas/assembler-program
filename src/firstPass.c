#include "../include/firstPass.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tables.h"
int firstPartExe(char *fileNmes)
{
    int IC, DC, res, L,i, symbolCount = 0,dirCount=0,errFlag=0,err=-1,symbolFlag=-1,externsCounter=0,intrucsCounter=0;
    size_t symTableCap=10, dirCapacity=10,instCapactiy=10;
    Symbol *symbolTable= malloc(symTableCap * sizeof(Symbol)), symbol;
    Directive *directives= malloc(dirCapacity * sizeof(Directive)),directiveInst;
    Instruction *instrucs= malloc(instCapactiy * sizeof(Instruction));
    FILE *fp;
    char line[MAX_LINE_LENGTH], *str,*pos;
    IC = 100;
    DC = 0;
    Instruction instruction;
    fp= fopen(fileNmes,"r");
    if(fp==NULL)
    {
        //throw error
        return -1;
    }
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (strcmp(line, "\n") == 0) {
            continue;
        }
        pos= strchr(line, '.');
        if(pos)
        {
            sscanf(pos, "%s", str);
            res = isDirective(str);
            extractSymbol(line,&symbol,res);
            //TO DO:CHECK IF I NEED TO RESET SYMBOL EVERY TIME
            if (symbol.name[0] == '\0' != 0 )
            {
                if(!validSymbol(&symbol,symbolTable,symbolCount))
                {
                    //TO DO:   not a valid symbol throw error
                } else
                {
                    symbolFlag=1;
                }
            }
            switch (res)
            {
                case 1:
                case 2:
                {
                    processDataOrStr(res,&directiveInst,line,&err);
                    insertDir(&directiveInst, &directives, &err, &dirCapacity, &dirCount);
                    if(symbolFlag==1)
                    {
                        insertSymbol(&symbolTable,&symbol,&symbolCount,&symTableCap,DC);

                        symbolFlag=0;
                    }
                    //CHECK IF I ADDED +1 IN LEN FOR STRING
                    DC+=directiveInst.len;
                    break;
                }
                case 4:
                {
                    //its entry, will be handeled in second pass so just skip to the next line
                    break;
                }
                case 3:
                {
                    //  its extern
                    //TO DO
                    if(symbolFlag==1)
                    {
                        insertSymbol(&symbolTable,&symbol,&symbolCount,&symTableCap,0);
                        externsCounter++;
                        symbolFlag=0;
                    }
                    break;
                }
                default:
                {

                        //TO DO: THIS WILL FILL THE INSTRUCTION DETAILS
                        if (parseInstruction(line, &instruction,IC,&err,&symbolFlag) == 0)
                        {
                            //the instruction has something illegal
                            //ERROR
                            continue;

                        } else
                        {
                            insertInstruction(&instruction,&instrucs,&instCapactiy,&intrucsCounter);
                            L = instruction.wordCount;
                            buildFirstWord();//TO DO
                            if(instruction.wordCount>1)
                            {
                                buildWords();//TO DO
                            }
                            IC+=L;
                        }
                    //its a normal instructuin
                    if(symbolFlag==1)
                    {
                        insertSymbol(&symbolTable,&symbol,&symbolCount,&symTableCap,IC);
                        symbolFlag=0;
                    }



                }
            }


        }
    }
    fclose(fp);
}


