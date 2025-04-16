#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
/* malloc handeling functions: */

void* handleMalloc(int size)
{
    void* tmp= malloc(size);
    if(tmp==NULL)
    {
        //TO DO
        //throw error allocating memory
    }
    return tmp;
}

/* file handeling functions: */
//////////////////////////////////////////maybe change the file name to fixed mac len 256 and not malloc
char* addFile(const char *fileName, const char *extenstion)
{
    char *dot,*fullName;
    int len=MAX_LINE_LENGTH*sizeof(char);
    fullName=handleMalloc(len);

        dot= strchr(fullName,'.');
        if(dot !=NULL)
        {
            //TO DO
            //throw error wrong file name
        }
    strcat(fullName, extenstion);
    return fullName;

}

void removeWhitespace(char* line)
{
    int i = 0, j = 0, inToken = 0; // Flag to track if we're inside a token

    while (line[i])
    {
        if (!isspace(line[i]))
        {

            if (inToken && j > 0 && !isspace(line[j - 1]))
            {
                line[j++] = ' ';
            }
            line[j++] = line[i];
            inToken = 1;
        } else
        {
            inToken = 0;
        }
        i++;
    }
    line[j] = '\0';
}


void generateOutputFilename(const char* inputFilename, char* outputFilename, const char* newExtension)
{

    strcpy(outputFilename, inputFilename);
    char* lastDot = strrchr(outputFilename, '.');
    if (lastDot != NULL)
    {
        strcpy(lastDot, newExtension);
    } else
    {
        strcat(outputFilename, newExtension);
    }
}
int isCommentLine(const char *line)
{
    while (isspace(*line))
        line++;
    return *line== ';';
}
int isEmptyLine(const char *line)
{
    while (*line)
    {
        if(!isspace(*line))
            return 0;
        line++;
    }
    return 1;
}
