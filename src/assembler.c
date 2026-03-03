#include <stdio.h>
#include <stdlib.h>
#include "../include/assembler.h"
#include "../include/firstPass.h"

/**
 * @brief Main entry point for the assembler program
 *
 * Processes each assembly file passed as command line arguments through:
 * 1. Pre-assembler (mcro expansion)
 * 2. First pass of assembly
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments (input filenames)
 * @return int Program exit status (0 for success)
 */

int main(int argc, char *argv[])
{
    char *asFile,*amFile;
    int i;
    /* Validate at least one input file was provided */
    if (argc < 2)
    {
        fprintf(stderr, "Error: No input files specified\n");
        return 1;
    }

    /* Process each input file */
   for( i=1;i<argc;i++)
   {
       printf("pre processing :\n");

       /* Add .as extension */
       asFile=addFile(argv[i],AS_EXT);
       if (!asFile)
       {
           fprintf(stderr, "Error: Memory allocation failed for filename\n");
           continue;
       }

       /* Execute pre-assembler */
       if(preExec(asFile))
       {
           /* Execute first pass */
           printf("first pass: \n");
           amFile=addFile(argv[i],AM_EXT);
           if(firstPartExe(amFile))
           {
               //TO DO: IF SECOND PART SUCCEEDED GENERTAE OUT FILES
               /*
               if(secondPartExe(amFile))
               {
                   generateOutputFiles
               }
                */

           }

           free(amFile);
       }
       free(asFile);

   }

    printf("end\n");
    return 0;
}
