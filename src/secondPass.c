#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/secondPass.h"
#include "../include/utils.h"
#include "../include/errors.h"
int secondPartExec(char *file_name, Symbol *symbolTable , int IC, int DC, int symbolCount, int externsCounter,int entriesCounter,
                   MachineWord *dataWords, extEntTable *externs, extEntTable *entries, int err, Instruction *instrcs, int instrcsCount, int L)
    {
    MachineWord *externsWords, *codeWords,*entryWords;
    int exWordsCounter;
    size_t exWordsCap;
        exWordsCap=10,exWordsCounter=0;

        /* Create machine code words from instructions */
        codeWords= createCodeWords(instrcs,instrcsCount,L);
        if (!codeWords)
        {
            handleError(ERR_MEM_ALLOC,0,"");
            return 0;
        }

        /*checks defined externs / undefined entries
         * seperate functions*/
        if(checkSymbolTables(symbolTable,externs,entries,entriesCounter,externsCounter,symbolCount,&err)==0)
        {
            free(codeWords);
            return 0;
        }

        /*sets the labels machine words */
        if(setInstLabelsMw(symbolTable,symbolCount,&err,instrcs,instrcsCount,&externsWords,&externsCounter,&exWordsCap)==0)//TO DO
        {
            free(codeWords);
            return 0;
        }
        entryWords=createEntWords(entries,entriesCounter,symbolTable,symbolCount);
if(entryWords==NULL)
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
            printExtEntTable(entryWords,entriesCounter,".ent",file_name);
        }
        if(externsCounter!=0)
        {
            printExtEntTable(externsWords,exWordsCounter,".ext",file_name);
        }

        free(codeWords);
        free(externsWords);
        return !err;
    }
    int writeMWordsToHexObFile(const char *filename, MachineWord *codeWords, int codeLen,MachineWord *dataWords,int dataLen)
    {
    int i;
    char *obFileName;
    obFileName= addFile(filename,OBJ_EXT);
    FILE *fp = fopen(obFileName, "w");
    if (!fp)
    {
        free(obFileName);
        handleError(ERR_OPENING_FILE,0,"");
        return 0;
    }
        fprintf(fp, "%d %d\n", codeLen, dataLen);
        for (i = 0; i < codeLen; i++)
        {
            fprintf(fp, "%04d %03X\n", 100 + i, codeWords[i].word & 0xFFF);
        }

        /* Data section (continues after code section) */
        for (i = 0; i < dataLen; i++)
        {
            fprintf(fp, "%04d %03X\n", 100 + codeLen + i, dataWords[i].word & 0xFFF);
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
    int index,i,j,wordSlot,dist,extraWordAddr;
    for(i=0;i<insCount;i++)
    {
        wordSlot=1;
        for(j=0;j<instrucs[i].numOperand;j++)
        {
            if(instrucs[i].mode[j]==MODE_REGISTER)
            {
            
               if(instrucs[i].numOperand==2&&instrucs[i].mode[0]==MODE_REGISTER &&instrucs[i].mode[1]==MODE_REGISTER)
                {
                    if(j==0)
                    {wordSlot++;
                    } 
                }
                else
                {
                    wordSlot++;
                }
                continue;
            }
            if(instrucs[i].mode[j]==MODE_IMMEDIATE)
            {
                wordSlot++; 
                continue;
            }
            index=searchSymByName(symTable,symCount,instrucs[i].labelName[j]);
            if(index==-1)
            {
                *err = 1;
                /*TO DO CHECK IF HANDELING ERROR NEED INSTEAD OF PRINTING ERROR*/
                fprintf(stderr,"Error: undefined label '%s'\n", instrucs[i].labelName[j]);
                wordSlot++;
                continue;
            }
             extraWordAddr= instrucs[i].address+wordSlot;

            if(instrucs[i].mode[j]==MODE_RELATIVE)
            {
                dist=symTable[index].value-instrucs[i].address;
                instrucs[i].words[wordSlot].word= dist& 0xFFF;
                instrucs[i].words[wordSlot].are='A';
            }
            else  
            {
                if(symTable[index].isExternal==1)
                {
                    instrucs[i].words[wordSlot].word=0;
                    instrucs[i].words[wordSlot].are= 'E';
                    if(insertExternWord(externsCount, externCap, externWords, err,extraWordAddr, symTable[index].name) == 0)
                    {
                        return 0;
                    }
                }
                else
                {
                    instrucs[i].words[wordSlot].word= symTable[index].value & 0xFFF;
                    instrucs[i].words[wordSlot].are='R';
                }
            }
            wordSlot++;
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
MachineWord* createEntWords(extEntTable *ents, int totalWords,Symbol *symTable, int symCount)
{
    int i,idx;
    /* Allocate the codeWords array */
    MachineWord *entWords = (MachineWord*)malloc(totalWords * sizeof(MachineWord));
    if (!entWords)
    {
        return NULL;  /* Allocation failed */
    }
    for(i=0;i<totalWords;i++)
    {
        idx=searchSymByName(symTable,symCount, ents[i].name);
        strcpy(entWords[i].name,ents[i].name);
        entWords[i].word=(idx!= -1)? symTable[idx].value:0;
        entWords[i].are='A';

    }
    return entWords;

}