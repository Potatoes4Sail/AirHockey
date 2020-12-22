#ifndef SIDE_RACER_CHARFUNCTIONS_H
#define SIDE_RACER_CHARFUNCTIONS_H

#include <iostream>

int lengthOfChars(const char *charArray);
bool compareChars(const char *firstArray, const char *secondArray);
void getLengthOfFile(std::ifstream &fileStream, unsigned int &lineCount);
unsigned int countMatches(std::ifstream &fileStream, const char *string);

#endif //SIDE_RACER_CHARFUNCTIONS_H
