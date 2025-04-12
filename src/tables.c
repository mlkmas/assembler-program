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
        if (!resizeTable((void **)symbolTable, capacity, sizeof(Symbol *)))
        {
           //TO DO ERROR RESIZING
            return;
        }
        (*symbolSize)=capacity;
    }
    Symbol *newSym = malloc(sizeof(Symbol));
    if (!newSym)
    {
        return; // Allocation failed
    }
    // Copy data
    strcpy(newSym->name, symbol->name);
    newSym->value = symbol->value;
    newSym->isEntry = symbol->isEntry;
    newSym->isExternal = symbol->isExternal;
    newSym->isData = symbol->isData;
    newSym->lineNum=DC;
    // Add to array
    symbolTable[*symbolCount] = newSym;
    (*symbolCount)++;
}
void insertDir(Directive *dInst,Directive **directives, int *err, size_t *dirCapacity,int *dirCount)
{
    size_t capacity=(*dirCapacity);
    if ((size_t) dirCount >= capacity)
    {
        capacity*= 2;  // Double capacity (common strategy)
        if (!resizeTable((void **) directives, capacity, sizeof(Directive)))
        {
            //TO DO ERROR RESIZING
            return;
        }
        (*dirCapacity)=capacity;
    }
    Directive *newDir = malloc(sizeof(Directive));
    if (!newDir)
    {
        return; // Allocation failed
    }
    newDir->nums = dInst->nums;
    newDir->len = dInst->len;
    newDir->isData = dInst->isData;
    if (dInst->str != NULL)
    {
        newDir->str = malloc(strlen(dInst->str) + 1);
        if (!newDir->str) {
            free(newDir);
            return; // String allocation failed
        }
        strcpy(newDir->str, dInst->str);
    } else {
        newDir->str = NULL;
    }
    directives[*dirCount] = newDir;
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
    int c,i,numIndex,sign,currNum,flag; //the flag is 0 in case it shouldnt be a ,
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
    char *token;
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
    int comma,opCount;
    size_t length;
    char lineCopy[256], *token;
    strncpy(lineCopy, line, sizeof(lineCopy));
    lineCopy[sizeof(lineCopy) - 1] = '\0';
    token= strtok(lineCopy," \t\n");

    if(symbolFlag==1)/*that means the first word is a symbol, skip it*/
    {
        token= strtok(NULL," \t\n");
    }
    /*token contains the instruction name*/
    if(token == NULL || setInstOp(instruc, token) == 0)
    {
        /*it didnt find a suitable instruction,TO DO: handle error*/
        return 0;
    }
    instruc->address=IC;
    instruc->wordCount=1;

    comma=0;//comma=0- a comma cant appear here, comma=1- comma should appear
    opCount=0;
    instruc->operands[opCount].labelName[0]='\0';
    while ((token = strtok(NULL, " \t\n")) != NULL)
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
            continue;//get the next token
        }
        /*check for comma at start */
            if(token[0]==',')
            {
                if(comma==0)
                {
                    /*error comma*/
                    return 0;
                }
                /*Remove comma from start*/
                memmove(token, token + 1, strlen(token));
                comma = 1;
            } // Case 3: Token doesn't start with comma but we expected one
            else if (comma == 1)
            {
               // *err = ERR_MISSING_COMMA;
                return 0;
            }
        // Check for trailing comma
         length = strlen(token);
        if (length > 0 && token[length-1] == ',')
        {
            // Remove comma from end
            token[length - 1] = '\0';
            comma = 0;
        } else {
            comma = 1;
        }
                /*now nadle intruc*/
                /*handle immediate*/
                if(token[0]=='#')
                {
                    /*its an immediate value*/
                    instruc->operands[opCount].mode=MODE_IMMEDIATE;
                    //extract num after /make sure there is one#
                    if (extractNum(&instruc->operands[opCount].imm, token + 1) == 0)
                    {
                       // *err = ERR_INVALID_IMMEDIATE;
                        return 0;
                    }
                    instruc->operands[opCount].labelAddress = -1;
                    instruc->operands[opCount].reg = -1;
                    instruc->wordCount++;

                } else if(token[0]=='&')/*handle relative label*/
                      {
                          instruc->operands[opCount].mode=MODE_RELATIVE;
                          instruc->operands[opCount].reg=-1;
                          //TO DO CHECK LABEL NAME SIZE
                          strncpy(instruc->operands[opCount].labelName, token + 1, 30);
                          instruc->operands[opCount].labelName[30] = '\0';
                          instruc->wordCount++;
                      } else {
                    instruc->operands[opCount].reg= isReg(token);
                    if(instruc->operands[opCount].reg==-1)
                    {
                        //its not a reg then its a label
                        instruc->operands[opCount].mode=MODE_DIRECT;
                        //TO DO : CHECK THAT TOKEN IS <31
                        strncpy(instruc->operands[opCount].labelName, token, 30);
                        instruc->operands[opCount].labelName[30] = '\0';
                        instruc->wordCount++;
                    } else{ /*its a reg*/
                        instruc->operands[opCount].labelAddress=-1;
                        instruc->operands[opCount].mode=OP_REGISTER;
                    }
                }

            opCount++;


    }
    if (opCount < instruc->inst->numOperands)
    {
       // *err = ERR_TOO_FEW_OPERANDS;
        return 0;
    }
    //TO DO, IMPLEMENT addressingOps functions that checks if the addressing methodes are correct
    if (addressingOps(instruc) == 0)
    {
       // *err = ERR_ILLEGAL_ADDRESSING;
        return 0;
    }

    return 1;
}

int extractNum(int *num, const char *str)
{
    char *endptr;
    long value;

    // Check for empty string
    if (*str == '\0')
    {
        return 0;
    }
    // Convert string to number
    value = strtol(str, &endptr, 10);

    // Check for conversion errors
    if (endptr == str)
    {  // No digits were converted
        return 0;
    }

    // Check for trailing non-whitespace characters
    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr))
        {
            return 0;
        }
        endptr++;
    }
    // Check for overflow/underflow
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

        if (!resizeTable((void **) instrucs, capacity, sizeof(Instruction*)))
        {
            //TO DO ERROR RESIZING
            return 0;
        }
        (*instCapactiy)=capacity;
    }
    Instruction *newInst = malloc(sizeof(Instruction));
    if (!newInst)
    {
        // Handle memory allocation failure
        return 0;
    }

    // Copy the instruction data
    *newInst = *instruction;

    // Add to the array
    instrucs[*intrucsCounter] = newInst;
    (*intrucsCounter)++;
    return 1;
}
void buildFirstWord(Instruction *ins,int *err)
{

    uint32_t word = 0;
    int ARE;
    AddressingMode srcMode,destMode;
    if(ins->inst->numOperands==0)
    {
        srcMode=0;
        destMode=0;
    }
    else{
        srcMode= (ins->inst->numOperands==2)?ins->operands[0].mode :0;
        destMode= (ins->inst->numOperands==2)?ins->operands[1].mode :ins->operands[0].mode;

    }
    ARE=001;
//TO DO MAKE SURE TO GIVE REG=0 IN CASE THERE WASNT AND NOT -1
    word |= (ins->inst->opcode & 0x3F) << 18;     // Opcode (bits 23-18)
    word |= (srcMode & 0x3) << 16;      // Source mode (bits 17-16)
    word |= (ins->operands[0].reg & 0x7) << 13;           // Source reg (bits 15-13)
    word |= (destMode & 0x3) << 11;     // Dest mode (bits 12-11)
    word |= (ins->operands[1].reg & 0x7) << 8;            // Dest reg (bits 10-8)
    word |= (ins->inst->funct & 0x1F) << 3;        // Funct (bits 7-3)
    word |= (ARE & 0x7);

    // Store in the struct (only 24 bits)
    ins->words[0].word = word & 0xFFFFFF; // Mask to 24 bits
}
void buildLabelMW(Instruction *ins,int *err,int add,int i)
{
    ins->words[i].word= add& 0xFFFFFF;

}
void setDataMWord(MachineWord **mw, int *wordsCount, int *err, Directive *dir)
{
    if (!mw || !dir || wordsCount < 0)
    {
       //TO DO  if (err) *err = 1;
        return;
    }

    int i;
    if (dir->isData)
    {
        // Process .data directive
        for (i = 0; i < dir->len; i++)
        {
            (*mw)[*wordsCount + i].word = dir->nums[i] & 0xFFFFFF;  // Ensure only 24 bits are stored
        }
    } else {
        // Process .string directive
        for (i = 0; i < dir->len; i++)
        {
            (*mw)[*wordsCount + i].word = (uint8_t)dir->str[i]; // Each char becomes 24-bit word (lower 8 bits used)
        }
    }

    *wordsCount += dir->len;
    if (err) *err = 0; // success
}