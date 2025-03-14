#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *asFile,*amFile;
   for(int i=0;i<argc;i++)
   {
       printf("pre processing :\n");
       asFile=addFile(argv[i],".as"); //TO DO
       if(macroExec(asFile)) //TO DO
       {
           printf("first pass: \n");
           amFile=addFile(argv[i],".am");
           if(!exeFirstPass(amFile))
           {
               free(asFile);
               free(amFile);
           }
       }

   }

    printf("end\n");
    return 0;
}
