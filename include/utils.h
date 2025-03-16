#ifndef UNTITLED6_UTILS_H
#define UNTITLED6_UTILS_H
#include "constants.h"
/***
 *
 * @param fileName
 * @param extenstion
 * @return
 */
char* addFile(char *fileName, char *extenstion);
/***
 * remove white spaces from the line
 * @param line
 */
void removeWhitespace(char* line);
/***
 *
 * @param inputFilename
 * @param outputFilename
 * @param newExtension
 */
void generateOutputFilename(const char* inputFilename, char* outputFilename, const char* newExtension);
int isEmptyLine(const char *line);
int isCommentLine(const char *line);

#endif /*UNTITLED6_UTILS_H */
