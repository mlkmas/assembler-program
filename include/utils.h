#ifndef UNTITLED6_UTILS_H
#define UNTITLED6_UTILS_H
#include "defines.h"


/*
 * Concatenates file name and extension, returns new string.
 * @param fileName - base file name
 * @param extension - file extension
 * @return pointer to newly allocated full file name
 */
char* addFile(const char *fileName, const char *extenstion);


/*
 * Removes extra whitespaces between tokens in a line.
 * @param line - input line to clean
 */
void removeWhitespace(char* line);

/*
 * Creates an output filename by changing extension.
 * @param inputFilename - original file name
 * @param outputFilename - buffer to store new file name
 * @param newExtension - new extension to apply
 */
void generateOutputFilename(const char* inputFilename, char outputFilename[MAX_FILE_NAME], const char* newExtension);

/*
 * Checks if a line is empty.
 * @param line - input line
 * @return 1 if empty, 0 otherwise
 */
int isEmptyLine(const char *line);

/*
 * Checks if a line is a comment line.
 * @param line - input line
 * @return 1 if comment, 0 otherwise
 */
int isCommentLine(const char *line);

#endif /*UNTITLED6_UTILS_H */
