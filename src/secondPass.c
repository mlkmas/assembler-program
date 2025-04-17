#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/secondPass.h"
#include "../include/utils.h"
int secondPartExec(char *file_name, Symbol *symbolTable , int IC, int DC, int symbolCount, int externsCounter,int entriesCounter,
                   MachineWord *dataWords, extEntTable *externs, extEntTable *entries, int err, Instruction *instrcs, int instrcsCount, int L)
    {
    MachineWord *externsWords, *codeWords,*entryWords;
    int exWordsCounter;
    size_t exWordsCap;
        exWordsCap=10,exWordsCounter=0;

        codeWords= createCodeWords(instrcs,instrcsCount,L);
        if (!codeWords)
        {
           //err
        }

        /*checks defined externs / undefined entries
         * seperate functions*/
        if(checkSymbolTables(symbolTable,externs,entries,entriesCounter,externsCounter,symbolCount,&err)==0)
        {
            //error
        }

        /*sets the labels machine words */
        if(setInstLabelsMw(symbolTable,symbolCount,&err,instrcs,instrcsCount,&externsWords,&externsCounter,&exWordsCap)==0)//TO DO
        {
            //error
        }
if(createEntWords(entries,entriesCounter)==0)
{
    //error
}
        /*ob file*/
        if(writeMWordsToHexObFile(file_name,codeWords,L,dataWords,DC)==0 )
        {
            //error
        }
        if(entriesCounter!=0)
        {
            //TO DO FILL ENTRY WORDS FIRST
            printExtEntTable(entryWords,entriesCounter,"ent",file_name);
        }
        if(externsCounter!=0)
        {
            printExtEntTable(externsWords,exWordsCounter,"ext",file_name);
        }

        //TO DO FREE EVRYTHING
        return !err;
    }
    int writeMWordsToHexObFile(const char *filename, MachineWord *codeWords, int codeLen,MachineWord *dataWords,int dataLen)
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
        fprintf(fp, "%d %d\n", codeLen, dataLen);
        for (i = 0; i < codeLen; i++)
        {
            fprintf(fp, "%06X %06X\n", 100 + i, codeWords[i].word & 0xFFFFFF);
        }

        /* Data section (continues after code section) */
        for (i = 0; i < dataLen; i++)
        {
            fprintf(fp, "%06X %06X\n", 100 + codeLen + i, dataWords[i].word & 0xFFFFFF);
        }

        fclose(fp);
        free(obFileName);
        return 1;
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
int setInstLabelsMw(Symbol *symTable,int symCount,int *err,Instruction *instrucs,int insCount,MachineWord **externWords,int *externsCount,size_t *externCap)
{
    int index,i,j,externIndex;
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
                    buildLabelMW(&instrucs[i],symTable[index].lineNum,j);
                   if(symTable[index].isExternal==1)
                   {
                      if(  insertExternWord(externsCount,externCap,externWords,err,instrucs[i].address+j,symTable[index].name)==0)
                      {
                          //err
                          return 0;
                      }
                   }

                }
            }
        }

    }
    return 1;

}

int searchSymByName(Symbol *symTable,int symCount,char *name)
{
    int i;
    for(i=0;i<symCount;i++)
{
        if(strcmp(symTable[i].name,name)==0)
       {
       return i;
        }
}
return -1;
}
int insertExternWord(int *externsCount,size_t *externCap,MachineWord **externWords,int *err,int address,char name[32])
{
    size_t newSize;
    newSize=(*externCap)*2;
    if(*externsCount>=*externCap)
    {
       if( resizeTable((void **)externWords,newSize,sizeof (MachineWord))==0)
       {
           //err
           return 0;
       }
    }
    (*externsCount)++;
    buildExternMW(externWords,address,*externsCount);
    strcpy(externWords[*externsCount]->name,name);
    (*externCap)=newSize;
    return 1;
}
void buildExternMW(MachineWord **externWords,int add,int i)
{
    externWords[i]->word= add& 0xFFFFFF;
}

MachineWord* createCodeWords(Instruction *instrcs, int instrcsCount, int totalWords)
{
    int i, j;


    /* Allocate the codeWords array */
    MachineWord *codeWords = (MachineWord*)malloc(totalWords * sizeof(MachineWord));
    if (!codeWords)
    {//err
        return NULL;  /* Allocation failed */
    }

    /* Copy words from each instruction */
    int wordIndex=0;
    for (i = 0; i < instrcsCount; i++)
    {
        for (j = 0; j < instrcs[i].wordCount; j++)
        {
            codeWords[wordIndex++] = instrcs[i].words[j];
        }
    }
    return codeWords;
}
int printExtEntTable(MachineWord *words,int len, char *extension,char *fileName)
{
    int i;
    char *newFileName;
    newFileName= addFile(fileName,extension);
    FILE *fp = fopen(newFileName, "w");
    if (!fp)
    {
        //error
        return 0;
    }
    for (i = 0; i < len; i++)
    {
        fprintf(fp, "%s %06X\n", words[i].name, words[i].word & 0xFFFFFF);
    }
    return 1;
}
MachineWord* createEntWords(extEntTable *ents, int totalWords)
{
    int i;
    /* Allocate the codeWords array */
    MachineWord *entWords = (MachineWord*)malloc(totalWords * sizeof(MachineWord));
    if (!entWords)
    {//err
        return NULL;  /* Allocation failed */
    }
    for(i=0;i<totalWords;i++)
    {
        strcpy(entWords[i].name,ents[i].name);
        entWords[i].word=ents[i].line;

    }
    return entWords;

}