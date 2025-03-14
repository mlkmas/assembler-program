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
    //didnt mention the file name has a
//    dot= strchr(fullName,'.');
//    /* switch the file extenstion */
//    if(dot !=NULL)
//        *dot='\0';
    strcat(fullName, extenstion);
    return fullName;

}