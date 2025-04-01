#include "../include/firstPass.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tables.h"
int firstPartExe(char *fileNmes)
{
    int IC, DC, res, L,i, symbolCount = 0,symTableSize=10,errFlag=0,errCode=-1;
    Symbol *symbolTable= malloc(symTableSize * sizeof(Symbol)), symbol;
    directives *directiveInst;
    FILE *fp;
    char line[MAX_LINE_LENGTH], str[MAX_LINE_LENGTH];
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
        if (strchr(line, '.'))
        {
            strcpy(str, line);
            res = isDirective(str);
            switch (res)
            {
                case 1:
                case 2:
                {
                    //TO DO: CHECKS IF THE DATA/STR HAS A STMBOL DEF IF YES AND LEGAL ADD IT TO THE SYMBOL TABLE
                    directiveInst= processDataOrStr(str,&errCode);
                    break;
                }
                case 4:
                {
                    //its entry, will be handeled in second pass so just skip to the next line
                    continue;
                    break;

                }
                case 3:
                {
                    //  its extern
                    //TO DO
                    processExternOrEntry();
                    break;
                }
                default:
                {
                    //its a normal instructuin
                    //TO DO: EXTRACT SYMBOL RETURNS THE SYMBOL NAME IF FOUND ELSE NULL
                    symbol = extractSymbol(line);
                    if (symbol != NULL)
                    {
                        if (validSymbol(symbol))
                        {
                            //if the instruction has a symbol insert it as code
                            insertSymbol();//TO DO
                        } else
                        {
                            //TO DO:   not a valid symbol throw error
                        }
                    } else {
                        //TO DO: THOS WILL FILL THE INSTRUCTION DETAILS
                        if (parseInstruction(line, &instruction) == 0)
                        {
                            //the instruction has something illegal
                            continue;

                        } else
                        {
                            L = 1;
                            for (i = 0; i < instruction.num_operands; i++) {
                                if (instruction.operands[i].mode != OP_REGISTER)
                                    L++;
                            }
                        }

                    }

                }
            }


        }
    }
    fclose(fp);
}


directives* processDataOrStr(char *str, int *errCode)
{
directives *d;
    d= malloc( sizeof (directives));
    if(d==NULL)
    {
        //error
        return NULL;
    }
    //TO DO
    //IM THINKING OF HANDELING ALL OF THE DIRECTIVES HER(DATA STR EXTERN ENTRY) BY ADDING A FIELD TO TYPE THEN IN THE FIRST PATH EXEC KUST CHECK THE DIR TYPE AND INSERT IN TABLES ACCORDINGLY


    return d;

}