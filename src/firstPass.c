#include "../include/firstPass.h"
#include "../include/secondPass.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tables.h"
#include "../include/errors.h"

int firstPartExe(char *fileName)
{
    int IC, DC, res, L,i, symbolCount = 0,dirCount=0,errFlag=0,err=-1,symbolFlag=-1,externsCounter=0,intrucsCounter=0,
    wordsCount=0,entriesCounter=0,lineNum=0;
    size_t symTableCap=10, dirCapacity=10,instCapactiy=10,entCap=10,exCap=10;
    extEntTable *externs,*entries; /* TO DO: ADD THE EXTERN TABLE EVERYWHERE  */
    externs=malloc(exCap * sizeof(extEntTable));
    entries=malloc(entCap * sizeof(extEntTable));
    Symbol *symbolTable= malloc(symTableCap * sizeof(Symbol)), symbol;
    MachineWord *dataMWs;
    Directive *directives= malloc(dirCapacity * sizeof(Directive)),directiveInst;
    Instruction *instrucs= malloc(instCapactiy * sizeof(Instruction));
    FILE *fp;
    char line[MAX_LINE_LENGTH], *str = NULL,*pos;
    char *entryName;
    char tmpLine[MAX_LINE_LENGTH];
    IC = 100;
    DC = 0;
    Instruction instruction;
    fp= fopen(fileName,"r");
    if(fp==NULL)
    {
        handleError(ERR_OPENING_FILE,0,fileName);
        return -1;
    }
   while (fgets(line, MAX_LINE_LENGTH, fp) !=NULL)
{
    lineNum++;
    line[strcspn(line, "\r\n")] = '\0';
    if(strlen(line) > 80 && line[80] != '\n' && line[80] != '\0')
{
    handleError(ERR_INVALID_ARGUMENT, lineNum, fileName);  /* or add ERR_LINE_TOO_LONG */
    errFlag = 1;
   
    while(line[strlen(line)-1] != '\n' &&fgets(line, MAX_LINE_LENGTH,fp)!= NULL);
    continue;
}
    if(isEmptyLine(line) ||isCommentLine(line))  
    {
        continue;
    }

    /* Check for label first */
    symbolFlag=-1;
    extractSymbol(line,&symbol,0);
    if(symbol.name[0] != '\0')
    {
        if(!validSymbol(&symbol,symbolTable,symbolCount))
        {
            errFlag=1;
        } else
        {
            symbolFlag=1;
        }
    }

    pos= strchr(line, '.');
    if(pos)
    {
        char dirToken[MAX_LINE_LENGTH];                  
        sscanf(pos, "%s", dirToken);
        res = isDirective(dirToken);
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
                DC+=directiveInst.len;
                break;
            }
            case 4:
            {
                strcpy(tmpLine, line);
                strtok(tmpLine, " \t\n"); /* skip ".entry" */
                entryName=strtok(NULL, " \t\n"); /* get the name */
                if (entryName)
                {
                     if(inserExternEntry(entries, &entriesCounter, entryName, &entCap, lineNum)==0)
                      {
                        handleError(ERR_MEM_ALLOC, lineNum, fileName);
                        errFlag=1;
                     }
    }
    break;
            }
            case 3:
            {
                char *externName;
                char tmpLine[MAX_LINE_LENGTH];
                strcpy(tmpLine, line);
                strtok(tmpLine, " \t\n");/* skip ".extern" */
                externName = strtok(NULL, " \t\n");
                if (externName)
                {
                    if(inserExternEntry(externs, &externsCounter, externName, &exCap, lineNum)==0)
                    {
                        handleError(ERR_MEM_ALLOC,lineNum, fileName);
                        errFlag=1;
                    }
                    strcpy(symbol.name,externName);
                    symbol.isExternal= 1;
                    symbol.isData= 0;
                    symbol.value =0;
                    symbol.isEntry =0;
                    insertSymbol(&symbolTable, &symbol, &symbolCount, &symTableCap, 0);
                }
                 symbolFlag=0;
                  break;
            }

            default:
                break;
        }
    }
    else  /* it's an instruction — this branch was unreachable before */
    {
        if (parseInstruction(line, &instruction,IC,&errFlag,symbolFlag) == 0)
        {
            fprintf(stderr, "DEBUG: parseInstruction failed on line %d: '%s'\n", lineNum, line);
            errFlag=1;
            continue;
        }
        buildFirstWord(&instruction,&errFlag);
        buildExtraWords(&instruction);
        L =instruction.wordCount;
        if(symbolFlag==1)
        {
            insertSymbol(&symbolTable,&symbol,&symbolCount,&symTableCap,IC);
            symbolFlag=0;
        }
        insertInstruction(&instruction,&instrucs,&instCapactiy,&intrucsCounter);
        IC+=L;
    }
}
    updateDataSymbols(symbolTable, symbolCount, IC);
    dataMWs= malloc(DC * sizeof(MachineWord));
    /*set data mwords*/
    for(i=0;i<dirCount;i++)
    {
        setDataMWord(&dataMWs,&wordsCount,&err,&directives[i]);
    }
    if(errFlag)
    {
    fclose(fp);
    return 0;   /* don't proceed to second pass if first pass had errors */
    }

    if( secondPartExec(fileName,symbolTable ,  IC,  DC,  symbolCount,  externsCounter, entriesCounter,
     dataMWs, externs, entries, err, instrucs,  intrucsCounter,  L)==0)
    {
        handleError(ERR_UNKNOWN,0,fileName);
        fclose(fp);
        return 0;
    }
     fprintf(stderr, "  DEBUG firstPartExe: about to return 1\n");
    return 1;
}


int inserExternEntry(extEntTable *table,int *count,char name[32],size_t *capacity,int line)
{
  size_t newSize;

 if(*count >=(int)*capacity)
  {
    newSize=(*capacity)*2;
     if( resizeTable((void **)table,newSize,sizeof (extEntTable))==0)
      {
      handleError(ERR_MEM_ALLOC,line, "");
       return 0;
      }
(*capacity)=newSize;
   }

strcpy(table[*count].name,name);
table [(*count)++].line=line;

return 1;
}