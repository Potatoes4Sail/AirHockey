#include "charFunctions.h"
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

int lengthOfChars(const char *charArray)  {
    int lengthOfString = 0;
    while (charArray[lengthOfString] != '\0'){
        lengthOfString++;
    }
    return lengthOfString;
}

bool compareChars(const char *firstArray, const char *secondArray){
    int lengthOfFirstArray;
    int lengthOfSecondArray;
    lengthOfFirstArray = lengthOfChars(firstArray);
    lengthOfSecondArray = lengthOfChars(secondArray);

    if (lengthOfFirstArray != lengthOfSecondArray) return false;

    for (int character = 0; character < lengthOfFirstArray; ++character) {
        if (firstArray[character] != secondArray[character]) {
            return false;
        }
    }
    return true;
}


void getLengthOfFile(ifstream &fileStream, unsigned int &lineCount) {
    fileStream.unsetf(ios_base::skipws); // Don't skip whitespace;
    lineCount = (unsigned int) std::count(std::istream_iterator<char>(fileStream), std::istream_iterator<char>(), '\n');
    // Gets length of file.
    lineCount++; // Last line lacks newline statement;

    // Re-ignore whitespace, clear any errors & and go back to start of file.
    fileStream.setf(ios_base::skipws);
    fileStream.clear();
    fileStream.seekg(0, ios::beg);
}

unsigned int countMatches(ifstream &fileStream, const char *string) {
    unsigned int matches = 0;
    char buffer[UCHAR_MAX];
    while (!fileStream.eof()){
        fileStream >> buffer;
        if(compareChars(string, buffer)) matches++;
    }

    fileStream.clear();
    fileStream.seekg(0, ios::beg);
    return matches;
}

