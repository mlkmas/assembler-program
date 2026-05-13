#include "../include/utils.h"
#include "../include/errors.h"
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
        sysError(ERR_MEM_ALLOC);
    }
    return tmp;
}

/* file handeling functions: */

char* addFile(const char *fileName, const char *extenstion)
{
    char *fullName;
    int len=(strlen(fileName)+strlen(extenstion)+1)*sizeof(char);
    fullName=handleMalloc(len);

        strcpy(fullName,fileName);
        
    strcat(fullName, extenstion);
    return fullName;

}

void removeWhitespace(char* line)
{
    int i = 0, j = 0, inToken = 0; /*Flag to track if we're inside a token*/

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


void generateOutputFilename(const char* inputFilename, char outputFilename[MAX_FILE_NAME], const char* newExtension)
{
    char* lastDot;

    strcpy(outputFilename, inputFilename);
    lastDot = strrchr(outputFilename, '.');
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
