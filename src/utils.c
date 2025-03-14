#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
/* malloc handeling functions: */


/* file handeling functions: */

char* addFile(char *fileName, char *extenstion)
{
    char *dot,*fullName;
    int len=MAX_LINE_LENGTH*sizeof(char);
    fullName=handleMalloc(len);

    strcat(fullName, extenstion);
    return fullName;

}