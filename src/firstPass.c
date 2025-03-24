#include "../include/firstPass.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int firstPartExe(char *fileNmes)
{
    int IC,DC,res;
    FILE *fp;
    char line[MAX_LINE_LENGTH],str[MAX_LINE_LENGTH],symbol[MAX_LINE_LENGTH];
IC=100;
DC=0;
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (strcmp(line, "\n") == 0) {
            continue;
        }
        if (strchr(line, '.'))
        {
            strcpy(str, line);
            res= isDirective(str);
            if(res==1 ||res==2) /*is it a data or string*/
            {
                //TO DO: CHECKS IF THE DATA/STR HAS A STMBOL DEF IF YES AND LEGAL ADD IT TO THE SYMBOL TABLE
                    processDataOrStr();
            }
            else if(res==4)
            {
                //its entry, will be handeled in second pass so just skip to the next line
                continue;
            } else if(res==3)
            {
              //  its extern
              //TO DO
              processExternOrEntry();

            } else{ //its a normal instructuin
                //TO DO: EXTRACT SYMBOL RETURNS THE SYMBOL NAME IF FOUND ELSE NULL
                symbol=extractSymbol(line)
                if(symbol!=NULL )
                {
                    if(validSymbol())
                    {
                        //if the instruction has a symbol insert it as code
                        insertSymbol();//TO DO
                    }
                    else{
                     //TO DO:   not a valid symbol throw error
                    }
                }
                else
                {

                }

            }


        }


    }
}