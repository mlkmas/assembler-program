#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/tables.h"

void insertSymbol(Symbol **symbolTable, Symbol *symbol, int *symbolCount,size_t *symbolSize,int DC) // ones digit is for entr, tens for ectern
{
    size_t capacity=(*symbolSize);
    if ((size_t) symbolCount >= capacity)
    {
        capacity*= 2;  // Double capacity (common strategy)
        if (!resizeTable((void **) &symbolTable, capacity, sizeof(Symbol)))
        {
           //TO DO ERROR RESIZING
            return;
        }
        (*symbolSize)=capacity;
    }
    strcpy(symbolTable[*symbolCount]->name, symbol->name);
    (*symbolTable)[*symbolCount].value = symbol->value;
    (*symbolTable)[*symbolCount].isEntry = symbol->isEntry ;
    (*symbolTable)[*symbolCount].isExternal = symbol->isExternal;
    (*symbolTable)[*symbolCount].isData = symbol->isData;
    (*symbolCount)++;
}
void insertDir(Directive *dInst,Directive **directives, int *err, size_t *dirCapacity,int *dirCount)
{
    size_t capacity=(*dirCapacity);
    if ((size_t) dirCount >= capacity)
    {
        capacity*= 2;  // Double capacity (common strategy)
        if (!resizeTable((void **) &directives, capacity, sizeof(Directive)))
        {
            //TO DO ERROR RESIZING
            return;
        }
        (*dirCapacity)=capacity;
    }
    directives[(*dirCount)]->nums=dInst->nums;
    directives[(*dirCount)]->len=dInst->len;
    directives[(*dirCount)]->isData=dInst->isData;
    if(dInst->str!=NULL)
       strcpy( directives[(*dirCount)]->str,dInst->str);
    else  directives[(*dirCount)]->str=NULL;
    (*dirCount)++;

}
int validSymbol(Symbol *symbol, Symbol symbolTable[], int symbolCount)
{
    int i;
    if (!symbol->name ||symbol->name[0]=='\0' || !isalpha(symbol->name[0]))
    {
        return 0;
    }
    for( i=0;symbol->name[i]!='\0';i++)
    {
        if(!isalnum(symbol->name[i]))
        {
            //error
            return 0;
        }
    }
    for (i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbolTable[i].name, symbol->name) == 0)
        {
            //TO DO PRINT error
            return 0; // Symbol already exists
        }
    }

    if(isInstruction(symbol->name)==1 )
    {
        //its not a valis name TO DO ERROR
        return 0;
    }
    return 1; // Valid symbol
}
void extractSymbol(char *line, Symbol *symbol, int isData)
{
    char lineCopy[256];
    char *token;
    // Make a copy of the line
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';
    // Initialize symbol defaults
    symbol->name [0]='\0';
    symbol->isExternal = 0;
    symbol->isData = (isData == 1 || isData == 2) ? 1 : 0;

    char *colonPos = strchr(lineCopy, ':');
    if (colonPos)
    {
        // Extract the first word (the symbol name)
        token = strtok(lineCopy, " \t\n"); // Get the first word
        if (token && token[strlen(token) - 1] == ':')
        {
            token[strlen(token) - 1] = '\0'; // Remove the trailing ':'
            strncpy(symbol->name, token, sizeof(symbol->name));
            symbol->name[sizeof(symbol->name) - 1] = '\0';
        } else
        {
            // Handle error: invalid symbol format

        }
    } else if (isData == 3)
    {
        // If no ':' and isData == 3, extract the second word
        token = strtok(lineCopy, " \t\n"); // Get the first word (.extern i dont need it)
        token = strtok(NULL, " \t\n");     // Get the second word
        if (token)
        {
            strncpy(symbol->name, token, sizeof(symbol->name));
            symbol->name[sizeof(symbol->name) - 1] = '\0'; // Ensure null-termination
        } else {
            // Handle error: no second word found

        }
    }
}
int resizeTable(void **table, size_t newSize, size_t elemCounter)
{
    void *newTable = realloc(*table, newSize * elemCounter);
    if (!newTable && newSize > 0) {
        return 0;  // Resize failed
    }
    *table = newTable;
    return 1;
}
void processDataOrStr(int res,Directive *directiveInst,char *line,int *err)
{
    //TO DO : CHECK IF I NEED TO STORE THE SYMBOL
    char lineCopy[256];
    char *token;
    // Make a copy of the line
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';
    if(res==1)//its .data jandle nums
    {
        if(extractNums(lineCopy,directiveInst,err)==0)
        {
            //handle error
        }
       directiveInst->str=NULL;
        directiveInst->isData=1;
    }
    else /*its a string*/
    {
        if(extractStr(lineCopy,directiveInst,err)==0)
        {
            //error
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
    int c,i,numIndex,sign,currNum,flag,comma; //the flag is 0 in case it shouldnt be a ,
    numIndex = 0;
     sign = 1;
    c= countNums(line);
    dir->len=c;
    if(c==0)
    {
        dir->nums= NULL;
        //error empty .data line
        return 0;
    }
    dir->nums =(int *)malloc(c * sizeof(int));
    if (dir->nums == NULL)
    {
        //error malloc
        return 0;
    }
    i=0;
    while (line[i] != '\0')
    {
        if ((isdigit(line[i]) && flag==0) || (line[i] == '-' && isdigit(line[i + 1])))
        {
            sign = 1;
            if (line[i] == '-')
            {
                sign = -1;
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
        } else { //its not a digit
            if(line[i]==',' && flag==1)
            {
                flag=0;
                i++;
            }
            else if(line[i]==' ')
                {
                i++;
                } else{
                //TO DO ERROR SYNTAX IN DATA
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
        // Skip leading non-digit characters (including spaces, commas, '.')
        if (isdigit(line[i]) || (line[i] == '-' && isdigit(line[i + 1])))
        {
            c++;
            // Skip the rest of the current number
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
    char *token,*colonPtr;
    int start,i;

    token = strtok(line, " ");
    if( strchr(token, ':') != NULL)
    {
        //its a label name, skip
        token = strtok(line, " ");
        if(strcmp(token, ".string") != 0)
        {
            //error in the syntax
            return 0;
        }

    } else {
        if(strcmp(token, ".string") != 0)
        {
            //error in the syntax
            return 0;
        }

    }
    token = strtok(NULL, "");//get the rest of the line
      for ( i = 0; token[i] != '\0'; i++)
      {
          if(token[i]=='"')
              break;
          if(token[i]!=' ')
          {
              //error in syntax before an " openeing
              return 0;
          }
      }
      if(token[i]=='\0')
      {
          //didnt find the opening "
          //error
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
        //didnt find the closing "
        //error
        return 0;
    }
    i++;
    dir->len=i-start+1;
    for(; token[i] != '\0'; i++)
    {
        if(token[i]!=' ' && token[i]!='\t' && token[i]!='\n')
        {
            //error trailing letters
            dir->len=0;
            return 0;
        }
    }
    dir->str=(char*)malloc(sizeof(char) * (dir->len + 1));
    if(dir->str ==NULL)
    {
        //error allocating memory
        return 0;
    }
    strncpy(dir->str, line + start, dir->len);
    dir->str[dir->len] = '\0'; // Null-terminate the new string
    return 1;
}

int parseInstruction(char *line, Instruction *instruc, int IC, int *err, int symbolFlag)
{
    int i,comma,opCount,num,res;
    char lineCopy[256], *token;
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';
    token= strtok(lineCopy," \t\n");

    if(symbolFlag==1)/*that means the first word is a symbol, skip it*/
    {
        token= strtok(NULL," \t\n");
    }
    /*token contains the instruction name*/
    if(setInstOp(&instruc, token) == 0)
    {
        /*it didnt find a suitable instruction,TO DO: handle error*/
        return 0;
    }
    instruc->address=IC;

    comma=0;//comma=0- a comma cant appear here, comma=1- comma should appear
    opCount=0;
    instruc->operands[opCount].labelName[0]='\0';
    while (token!=NULL)
    {
        if(opCount>=instruc->inst->numOperands)
        {
            /*error an additional op*/
            return 0;
        }

        if(strcmp(token,",")==0)
        { /*its a comma*/
            if(comma==0)
            {
                /*error an additional comma*/
                return 0;
            }
            comma=0;
        }
        else{
            if(token[0]==',')
            {
                if(comma==0)
                {
                    /*error comma*/
                    return 0;
                }
                else{
                    comma=0;
                    removeStartComma(token);
                }
            }
            else{
                if(comma==1)
                {
                    //error missing comma
                    return 0;
                }
                /*does it end with one*/
                if(token[i?]==',')
                {
                    removeEndComma(token);
                    comma=0;
                } else comma=1;

                /*now nadle intruc*/
                /*handle immediate*/
                if(token[0]=='#')
                {
                    /*its an immediate value*/
                    instruc->operands[opCount].mode=MODE_IMMEDIATE;
                    //extract num after /make sure there is one#
                    res=extractNum(&num, token);
                    if(res==0)
                    {
                        //error in imm value
                        return 0;
                    }
                    instruc->operands[opCount].imm=num;
                    instruc->operands[opCount].labelAddress=-1;
                    instruc->operands[opCount].reg=-1;

                } else if(token[0]=='&')/*handle relative label*/
                      {
                          instruc->operands[opCount].mode=MODE_RELATIVE;
                          instruc->operands[opCount].reg=-1;
                      } else {
                    instruc->operands[opCount].reg= isReg(token);
                    if(instruc->operands[opCount].reg==-1)
                    {
                        //its not a reg then its a label
                        instruc->operands[opCount].mode=MODE_DIRECT;
                        //TO DO : CHECK THAT TOKEN IS <31
                        strcpy(instruc->operands[opCount].labelName,token);
                    } else{ /*its a reg*/
                        instruc->operands[opCount].labelAddress=-1;
                        instruc->operands[opCount].mode=OP_REGISTER;
                    }
                }
            }
            opCount++;
        }


    }



}