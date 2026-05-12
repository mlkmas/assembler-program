#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>  
#include <stdio.h>
#include "../include/tables.h"
#include "../include/errors.h"
void insertSymbol(Symbol **symbolTable, Symbol *symbol, int *symbolCount,size_t *symbolSize,int DC) /* ones digit is for entr, tens for ectern  */
{
    size_t capacity=(*symbolSize);
    if ((size_t)(*symbolCount) >= capacity)
    {
        capacity*= 2;  /*Double capacity (common strategy)*/

        if (!resizeTable((void **)symbolTable, capacity, sizeof(Symbol)))
        {
            handleError(ERR_MEM_ALLOC,0,"");
            return;
        }
        (*symbolSize)=capacity;
    }
    

    (*symbolTable)[*symbolCount] = *symbol;
     (*symbolTable)[*symbolCount].value = DC;
    (*symbolCount)++;
}
void insertDir(Directive *dInst,Directive **directives, int *err, size_t *dirCapacity,int *dirCount)
{
    size_t capacity=(*dirCapacity);
    if ((size_t) dirCount >= capacity)
    {
        capacity*= 2;
        if (!resizeTable((void **) directives, capacity, sizeof(Directive)))
        {
            handleError(ERR_MEM_ALLOC,0,"");
            return;
        }
        (*dirCapacity)=capacity;
    }
   
    (*directives)[*dirCount] = *dInst;   /* copy struct by value */
    (*dirCount)++;
}
int validSymbol(Symbol *symbol, Symbol symbolTable[], int symbolCount)
{
    int i;
    if (symbol->name[0]=='\0' || !isalpha(symbol->name[0]))
    {
        return 0;
    }
    for( i=0;symbol->name[i]!='\0';i++)
    {
        if(!isalnum(symbol->name[i]))
        {
            handleError(ERR_INVALID_SYM_NAME,0,"");
            return 0;
        }
    }
    for (i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbolTable[i].name, symbol->name) == 0)
        {
            handleError(ERR_DUPLICATE_SYMBOL,0,"");
            return 0;
        }
    }

    if(isInstruction(symbol->name)==1 )
    {
        handleError(ERR_INVALID_INSTRUCTION_NAME,0,"");
        return 0;
    }
    return 1;
}
void extractSymbol(char *line, Symbol *symbol, int isData)
{
    char lineCopy[MAX_LINE_LENGTH];
    char *token;

    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';

    symbol->name [0]='\0';
    symbol->isExternal = 0;
    symbol->isData = (isData == 1 || isData == 2) ? 1 : 0;

    char *colonPos = strchr(lineCopy, ':');
    if (colonPos)
    {
        /*Extract the first word (the symbol name)*/
        token = strtok(lineCopy, " \t\n"); /* Get the first word*/
        if (token && token[strlen(token) - 1] == ':')
        {
            token[strlen(token) - 1] = '\0'; /* Remove the trailing ':' */
            strncpy(symbol->name, token, sizeof(symbol->name));
            symbol->name[sizeof(symbol->name) - 1] = '\0';
        } else
        {
            handleError(ERR_INVALID_SYM_FORMAT,0,"");

        }
    } else if (isData == 3)
    {
        /* If no ':' and isData == 3, extract the second word*/
        token = strtok(lineCopy, " \t\n"); /* Get the first word (.extern i dont need it)*/
        token = strtok(NULL, " \t\n");     /* Get the second word*/
        if (token)
        {
            strncpy(symbol->name, token, sizeof(symbol->name));
            symbol->name[sizeof(symbol->name) - 1] = '\0';
        } else {
            /* Handle error: no second word found  */
            handleError(ERR_INVALID_DATA_FORMAT,0,"");

        }
    }
}
int resizeTable(void **table, size_t newSize, size_t elemCounter)
{
    void *newTable = realloc(*table, newSize * elemCounter);
    if (!newTable && newSize > 0)
    {
        return 0;
    }
    *table = newTable;
    return 1;
}
void processDataOrStr(int res,Directive *directiveInst,char *line,int *err)
{

    char lineCopy[MAX_LINE_LENGTH];
    char *dataPart;

    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy)-1] = '\0';
    fprintf(stderr, "  DEBUG processDataOrStr: res=%d line='%s'\n", res, lineCopy);
    if(res==1)/*its .data handle nums*/
    {
        dataPart=strstr(lineCopy,".data");
        fprintf(stderr, "  DEBUG processDataOrStr: dataPart before skip='%s'\n", dataPart ? dataPart : "NULL"); /* find .data in the line */
        if (dataPart) {dataPart += strlen(".data");} /* skip past it */
        else {dataPart = lineCopy; }
         fprintf(stderr, "  DEBUG processDataOrStr: dataPart after skip='%s'\n", dataPart);  
        if(extractNums(dataPart,directiveInst,err)==0)
        {
            /*handleError(ERR_INVALID_DATA_FORMAT,0,"");*/
        }
       directiveInst->str=NULL;
        directiveInst->isData=1;
    }
    else /*its a string*/
    {
        if(extractStr(lineCopy,directiveInst,err)==0)
        {
            fprintf(stderr, "DEBUG: extractStr failed on input: '%s'\n", lineCopy);
       
            /* error  */
            return;
        }
        directiveInst->nums=NULL;
        directiveInst->isData=0;
    }

}


int extractNums(char *lineCopy, Directive *dir,int *err)
{
    char line[256];
    strncpy(line, lineCopy, sizeof(line));
    line[sizeof(line)-1]='\0';
     fprintf(stderr, "  DEBUG extractNums input: '%s'\n", line);
    int c,i,numIndex,sign,currNum,flag; /* the flag is 0 in case it shouldnt be a ,  */
    numIndex = 0;
     sign = 1;
     flag=0;
    c= countNums(line);
    fprintf(stderr, "  DEBUG extractNums countNums returned: %d\n", c); 
    dir->len=c;
    if(c==0)
    {
         dir->nums= NULL;
         fprintf(stderr, "  DEBUG extractNums FAIL D1: empty data line\n");
        handleError(ERR_INVALID_DATA_FORMAT,0,"");
        return 0;
    }
    dir->nums =(int *)malloc(c * sizeof(int));
    if (dir->nums == NULL)
    {
        fprintf(stderr, "  DEBUG extractNums FAIL D2: malloc failed\n");
        handleError(ERR_MEM_ALLOC,0,"");
        return 0;
    }
    i=0;
    while (line[i] != '\0')
    {
        if ((isdigit(line[i]) && flag==0) || (line[i] == '-' && isdigit(line[i + 1])) || (line[i] == '+' && isdigit(line[i + 1])))
        {
            sign = 1;
            if (line[i] == '-')
            {
                sign = -1;
                i++;
            }
            if (line[i] == '+')
            {
                sign = 1;
                i++;
            }
             currNum = 0;
            while (isdigit(line[i]))
            {
                currNum = currNum * 10 + (line[i] - '0');
                i++;
            }
            dir->nums[numIndex++] = currNum * sign;
            flag=1;
        } else { /*its not a digit*/
            if(line[i]==',' && flag==1)
            {
                flag=0;
                i++;
            }
            else if(line[i]==' ')
                {
                i++;
                } else{
                    fprintf(stderr, "  DEBUG extractNums FAIL D3: bad char '%c' at pos %d in '%s', flag=%d\n", line[i], i, line, flag);
                handleError(ERR_INVALID_DATA_FORMAT, 0, "");
                return 0;
            }

        }
    }
    return 1;
}
int countNums(const char *line)
{

    int c =0;
    int i = 0;
     
    while (line[i] != '\0')
    {
        /*Skip leading non-digit characters (including spaces, commas, '.')*/
        if (isdigit(line[i]) || (line[i] == '-' && isdigit(line[i + 1])))
        {
            c++;
            /*Skip the rest of the current number */
            while (isdigit(line[i]) || line[i] == '-')
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    return c;
}
int extractStr(char *lineCopy, Directive *dir,int *err)
{
    char line[256];
    strncpy(line, lineCopy, sizeof(line));
    char *token;
    int start,i;
    int strContentLen;

    token = strtok(line, " ");
    if( strchr(token, ':') != NULL)
    {
        /*its a label name, skip*/
        token = strtok(NULL, " ");
        if(strcmp(token, ".string") != 0)
        {
            handleError(ERR_INVALID_ARGUMENT,0, "");
            return 0;
        }

    } else {
        if(strcmp(token, ".string") != 0)
        {
            handleError(ERR_INVALID_ARGUMENT,0, "");
            return 0;
        }

    }
    token = strtok(NULL, "");/*get the rest of the line*/
      for ( i = 0; token[i] != '\0'; i++)
      {
          if(token[i]=='"')
              break;
          if(token[i]!=' ')
          {
             handleError(ERR_INVALID_ARGUMENT, 0, "");
              return 0;
          }
      }
      if(token[i]=='\0')
      {
          handleError(ERR_INVALID_ARGUMENT, 0, "");
          return 0;
      }
      i++;
      start=i;
    for ( ; token[i] != '\0'; i++)
    {
        if(token[i]=='"')
            break;

    }
    if(token[i]=='\0')
    {
         handleError(ERR_INVALID_ARGUMENT, 0, "");
        return 0;
    }
    strContentLen=i-start;
    i++;
    /*dir->len=i-start+1;*/
    for(; token[i] != '\0'; i++)
    {
        if(token[i]!=' ' && token[i]!='\t' && token[i]!='\n')
        {
            /*error trailing letters*/
            handleError(ERR_INVALID_ARGUMENT, 0, "");
            dir->len=0;
            return 0;
        }
    }
    dir->len = strContentLen + 1;
    dir->str=(char*)malloc(strContentLen+1);
    if(dir->str ==NULL)
    {
        handleError(ERR_MEM_ALLOC, 0, "");
        return 0;
    }
    strncpy(dir->str, line + start,strContentLen);
    dir->str[strContentLen] = '\0'; /*Null-terminate the new string*/
    return 1;
}

int parseInstruction(char *line, Instruction *instruc, int IC, int *err, int symbolFlag)
{
    int comma,opCount;
    size_t length;
    char lineCopy[256], *token;
    InstrucOp op; 
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';
    token= strtok(lineCopy," \t\n");
    fprintf(stderr, "  DEBUG parseInst: first token='%s' symbolFlag=%d\n", token ? token : "NULL", symbolFlag); 

    if(symbolFlag==1)/*that means the first word is a symbol, skip it*/
    {
        token= strtok(NULL," \t\n");
        fprintf(stderr, "  DEBUG parseInst: after-symbol token='%s'\n", token ? token : "NULL");
    }
    /*token contains the instruction name*/
    if(token == NULL || getInstructionOp(&op, token) == 0)
    {
        /*it didnt find a suitable instruction,TO DO: handle error*/
        /*handleError(ERR_INVALID_INSTRUCTION_NAME, 0, ""); TO DO REMOVE IT AFTER DEBUGGING*/
       fprintf(stderr, "  D1 fail token=%s\n", token ? token : "NULL");
    handleError(ERR_INVALID_INSTRUCTION_NAME, 0, "");
    *err = 1;
        return 0;
    }
    
    instruc->opcode =op.opcode;          
    instruc->funct = op.funct;           
    instruc->numOperand=op.numOperands;  

    instruc->address=IC;
    instruc->wordCount=1;

    comma=0;/* comma=0- a comma cant appear here, comma=1- comma should appear  */
    opCount=0;
    instruc->labelName[0][0]='\0';
    while ((token = strtok(NULL, " \t\n")) != NULL)
    {
        if(opCount>=instruc->numOperand)
        {
            /*error an additional op*/
           fprintf(stderr, "  D2 fail extra op token=%s opCount=%d need=%d\n", token, opCount, instruc->numOperand);
    handleError(ERR_INVALID_ARGUMENT, 0, "");
    *err = 1;
     return 0;
        }

        if(strcmp(token,",")==0)
        { /*its a comma*/
            if(comma==0)
            {
                /*error an additional comma*/
                handleError(ERR_INVALID_ARGUMENT, 0, "");  
                *err = 1;
                return 0;
            }
            comma=0;
            continue;/*get the next token*/
        }
        /*check for comma at start */
            if(token[0]==',')
            {
                if(comma==0)
                {
                    /*error comma*/
                    handleError(ERR_INVALID_ARGUMENT, 0, "");  
                    *err = 1;
                    return 0;
                }
                /*Remove comma from start*/
                memmove(token, token+1, strlen(token));
                comma= 1;
            } /*Case 3: Token doesn't start with comma but we expected one*/
            else if (comma == 1)
            {
               /**err = ERR_MISSING_COMMA; */
                handleError(ERR_INVALID_ARGUMENT, 0, "");  
                *err = 1;
                return 0;
            }
        /*Check for trailing comma */
         length = strlen(token);
        if (length > 0 && token[length-1] == ',')
        {
            /* Remove comma from end*/ 
            token[length -1]= '\0';
            comma = 0;
        } else
         {
            comma= 1;
        }
                /*now nadle intruc*/
                /*handle immediate*/
                if(token[0]=='#')
                {
                    /*its an immediate value*/
                    instruc->mode[opCount]=MODE_IMMEDIATE;
                    /*extract num after /make sure there is one# */
                    if (extractNum(&instruc->imm[opCount],token + 1) == 0)
                    {
                       /* *err = ERR_INVALID_IMMEDIATE; */
                       fprintf(stderr, "  DEBUG parseInst: FAIL at <description>, token='%s', opCount=%d, comma=%d\n", token ? token : "NULL", opCount, comma);
                        return 0;
                    }
                    
                    instruc->reg[opCount] = -1;
                    instruc->wordCount++;

                } else if(token[0]=='&')/*handle relative label*/
                      {
                          instruc->mode[opCount]=MODE_RELATIVE;
                          instruc->reg[opCount]=-1;
                          /*TO DO CHECK LABEL NAME SIZE */
                          strncpy(instruc->labelName[opCount], token + 1, 30);
                          instruc->labelName[opCount][30] = '\0';
                          instruc->wordCount++;
                      } 
                      else 
                      {
                    instruc->reg[opCount]= isReg(token);
                    if(instruc->reg[opCount]==-1)
                    {
                        /*its not a reg then its a label*/
                        instruc->mode[opCount]=MODE_DIRECT;
                        /*TO DO : CHECK THAT TOKEN IS <31 */
                        strncpy(instruc->labelName[opCount], token, 30);
                        instruc->labelName[opCount][30] = '\0';
                        instruc->wordCount++;
                    } else{ /*its a reg*/
                        instruc->mode[opCount]=MODE_REGISTER;
                        
                    }
                }

            opCount++;


    }
    if (opCount < instruc->numOperand)
    {
       /**err = ERR_TOO_FEW_OPERANDS;*/ 
       fprintf(stderr, "  D9 fail too few ops opCount=%d need=%d\n", opCount, instruc->numOperand);
    return 0;
    }
    /*TO DO, IMPLEMENT addressingOps functions that checks if the addressing methodes are correct*/
    if (addressingOps(instruc) == 0)
    {
       /**err = ERR_ILLEGAL_ADDRESSING; */
       fprintf(stderr, "  D10 fail addressingOps opcode=%d mode[0]=%d mode[1]=%d\n", 
            instruc->opcode, instruc->mode[0], instruc->mode[1]);
    return 0;
    }

    return 1;
}

int extractNum(int *num, const char *str)
{
    char *endptr;
    long value;

    /*Check for empty string*/
    if (*str == '\0')
    {
        return 0;
    }
    /*Convert string to number*/
    value = strtol(str, &endptr, 10);

    /*Check for conversion errors*/
    if (endptr == str)
    {  /*No digits were converted*/
        return 0;
    }

    /*Check for trailing non-whitespace characters*/
    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr))
        {
            return 0;
        }
        endptr++;
    }
    /*Check for overflow/underflow*/
    if (value > INT_MAX || value < INT_MIN) {
        return 0;
    }
    *num = (int)value;
    return 1;
}
int insertInstruction(Instruction *instruction,Instruction **instrucs,size_t *instCapactiy, int *intrucsCounter)
{
    size_t capacity=(*instCapactiy);
    if ((size_t)(*intrucsCounter) >= capacity)
    {
        capacity = (capacity == 0) ? 1 : capacity * 2;

        if (!resizeTable((void **) instrucs, capacity, sizeof(Instruction)))
        {
            
            handleError(ERR_MEM_ALLOC, 0, "");
            return 0;
        }
        (*instCapactiy)=capacity;
    }
    /**/
    (*instrucs)[*intrucsCounter] = *instruction;
    (*intrucsCounter)++; 
    
    return 1;
}
void buildFirstWord(Instruction *ins,int *err)
{

    int word = 0;
    int srcMode,destMode;
    if(ins->numOperand==0)
    {
        srcMode=0;
        destMode=0;
    }
    else{
        srcMode= (ins->numOperand==2)?ins->mode[0] :0;
        destMode= (ins->numOperand==2)?ins->mode[1] :ins->mode[0];

    }
   
/*TO DO MAKE SURE TO GIVE REG=0 IN CASE THERE WASNT AND NOT -1*/
    word |= (ins->opcode & 0xF) << 8;     
    word |= (ins->funct  & 0xF) << 4;    
    word |= (srcMode     & 0x3) << 2;
    word |= (destMode    & 0x3);

    
    ins->words[0].word = word & 0xFFF; 
     ins->words[0].are= 'A';
}
void buildLabelMW(Instruction *ins,int add,int i)
{
    ins->words[i].word= add& 0xFFFFFF;

}
void setDataMWord(MachineWord **mw, int *wordsCount, int *err, Directive *dir)
{
    if (!mw || !dir ||  !wordsCount)
    {
        handleError(ERR_NULL_POINTER, 0, "");
        return;
    }

    int i;
    if (dir->isData)
    {
        /*Process .data directive*/
        for (i = 0; i < dir->len; i++)
        {
            (*mw)[*wordsCount + i].word = dir->nums[i] & 0xFFFFFF;  /* Ensure only 24 bits are stored  */
        }
    } else {
        /*Process .string directive*/
        for (i = 0; i < dir->len; i++)
        {
            (*mw)[*wordsCount + i].word = (uint8_t)dir->str[i]; /* Each char becomes 24-bit word (lower 8 bits used)  */
        }
    }

    *wordsCount += dir->len;
    if (err) *err = 0; /*success*/
}


void updateDataSymbols(Symbol *symbolTable,int symbolCount,int ICF)
{
    int i;
    for(i = 0;i< symbolCount;i++)
    {
        if(symbolTable[i].isData)
        {
      symbolTable[i].value+=ICF;
        }
           
    }
}

void buildExtraWords(Instruction *ins)
{
    int wordIdx = 1;
    int i;

    /*both operands are registers that share one word */
    if(ins->numOperand ==2&& ins->mode[0]== MODE_REGISTER && ins->mode[1] ==MODE_REGISTER)
    {
        ins->words[1].word=(1 << ins->reg[0])|(1 << ins->reg[1]);
        ins->words[1].word &= 0xFFF;
        ins->words[1].are= 'A';
        ins->wordCount=2;
        return;
    }

    for(i=0;i<ins->numOperand;i++)
    {
        if(ins->mode[i]==MODE_IMMEDIATE)
        {
            ins->words[wordIdx].word=ins->imm[i]& 0xFFF;
            ins->words[wordIdx].are= 'A';
        }
        else if(ins->mode[i]==MODE_REGISTER)
        {
            ins->words[wordIdx].word=(1 << ins->reg[i]) & 0xFF;
            ins->words[wordIdx].are='A';
        }
        else  /* MODE_DIRECT or MODE_RELATIVE placeholdr*/
        {
            ins->words[wordIdx].word= 0;
            ins->words[wordIdx].are='A';   /* will be overwritten in secondpass */
        }
        wordIdx++;
    }
    ins->wordCount=wordIdx;
}