#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/secondPass.h"
#include "../include/utils.h"
int secondPartExec(char *file_name,  Symbol *symbolTable , int IC, int DC, int symbolCount,int externsCounter,
int entriesCounter, MachineWord *dataMWs, extEntTable *externs, extEntTable *entries, int err,Instruction *instrcs, int instrcsCount)
    {
    MachineWord *externsWords;


        /*checks for duplicate symbols, defined externs, undefined entries
         * seperate functions*/
        if(checkSymbolTables(symbolTable,externs,entries,entriesCounter,externsCounter,symbolCount,&err)==0)
        {
            //error
        }

        /*sets the labels mw in the instructions machine words */
        if(setInstLabelsMw()==0)//TO DO
        {
            //error
        }
        /*set externs Words appearances*/
        if(setExternMwords()==0)
        {
            //error
        }
        /*ob file*/
        if(writeMWordsToHexObFile(file_name,w)==0 ||)
        {
            //error
        }
        if(entriesCounter!=0)
        {
            printEntries();
        }
        if(externsCounter!=0)
        {
            printExterns();
        }

        //TO DO FREE EVRYTHING
        return !err;
    }
    int writeMWordsToHexObFile(const char *filename, MachineWord *words, int len)
    {
    int i;
    char *obFileName;
    obFileName= addFile(filename,"ob");
    FILE *fp = fopen(obFileName, "w");
    if (!fp)
    {
      //error
        return 0;
    }

    for (i = 0; i < len; i++)
    {
        fprintf(fp, "%06X\n", words[i].word);
    }

    fclose(fp);
}
int checkSymbolTables(Symbol *symbolTable,extEntTable *externs,extEntTable *entries,int entriesCounter,int externsCounter,int symbolCount,int *err)
{
    int i;
    for(i=0;i<symbolCount;i++)
    {
       if(searchSymbolInotherTables(&symbolTable[i],externs,externsCounter)==1 && symbolTable[i].isExternal==1)
       {
           //error a defined extern
       }
        if(searchSymbolInotherTables(&symbolTable[i],entries,entriesCounter)==1 && symbolTable[i].isEntry==0)
        {
            //error  undefined entry
        }

    }
    return 1;
}
int searchSymbolInotherTables(Symbol *symbol,extEntTable *table,int count)
{
    int i;
    for(i=0;i<count;i++)
    {
        if( strcmp(symbol->name,table[i].name) ==0)
            return 1;
    }
    return 0;
}
int setInstLabelsMw(Symbol *symTable,int symCount,int *err,Instruction *instrucs,int insCount)
{
    int index,i,j;
    for(i=0;i<insCount;i++)
    {
        for(j=0;j<instrucs[i].wordCount;j++)
        {
            if(instrucs[i].operands[j].mode==1 || instrucs[i].operands[j].mode==2 )
            {
                index=searchSymByName(symTable,symCount,instrucs[i].operands[j].labelName);
                if(index==-1)
                {
                   // err=undefinedLabel;
                    return 0;
                } else{
                    buildLabelMW(&instrucs[i],&err,index,j);

                }
            }
        }

    }


}