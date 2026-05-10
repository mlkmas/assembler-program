#include "../include/firstPass.h"
#include "../include/secondPass.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tables.h"
int firstPartExe(char *fileName)
{
    int IC, DC, res, L,i, symbolCount = 0,dirCount=0,errFlag=0,err=-1,symbolFlag=-1,externsCounter=0,intrucsCounter=0,
    wordsCount=0,entriesCounter=0,lineNum=0;
    size_t symTableCap=10, dirCapacity=10,instCapactiy=10,entCap=10,exCap=10;
    extEntTable *externs,*entries; //TO DO: ADD THE EXTERN TABLE EVERYWHERE
    externs=malloc(exCap * sizeof(extEntTable));
    entries=malloc(entCap * sizeof(extEntTable));
    Symbol *symbolTable= malloc(symTableCap * sizeof(Symbol)), symbol;
    MachineWord *dataMWs;
    Directive *directives= malloc(dirCapacity * sizeof(Directive)),directiveInst;
    Instruction *instrucs= malloc(instCapactiy * sizeof(Instruction));
    FILE *fp;
    char line[MAX_LINE_LENGTH], *str = NULL,*pos;
    IC = 100;
    DC = 0;
    Instruction instruction;
    fp= fopen(fileName,"r");
    if(fp==NULL)
    {
        //throw error
        return -1;
    }
   while (fgets(line, MAX_LINE_LENGTH, fp) !=NULL)
{
    lineNum++;
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
                if(inserExternEntry(entries,&entriesCounter,symbol.name,&entCap,lineNum)==0)
                {
                    //err
                }
                entriesCounter++;
                break;
            }
            case 3:
            {
                if(inserExternEntry(externs,&externsCounter,symbol.name,&exCap, lineNum)==0)
                {
                    //err
                }
                externsCounter++;
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
            errFlag=1;
            continue;
        }
        buildFirstWord(&instruction,&errFlag);
        L = instruction.wordCount;
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
    dataMWs= malloc(dirCount * sizeof(MachineWord));
    /*set data mwords*/
    for(i=0;i<dirCount;i++)
    {
        setDataMWord(&dataMWs,&wordsCount,&err,&directives[i]);
    }

    if( secondPartExec(fileName,symbolTable ,  IC,  DC,  symbolCount,  externsCounter, entriesCounter,
     dataMWs, externs, entries, err, instrucs,  intrucsCounter,  L)==0)
    {
        //err
        fclose(fp);
        return 0;
    }
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
      //err
       return 0;
      }
(*capacity)=newSize;
   }

strcpy(table[*count].name,name);
table [(*count)++].line=line;

return 1;
}