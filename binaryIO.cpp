//
// Created by Patrick on 2020-12-05.
//

#include "binaryIO.h"
#include "charFunctions.h"

using namespace std;

binaryIO::binaryIO(const char *_fileName) {
    emptyFile = false; // Sets to false and unless error occurs assume it will remain this way.


    // Opens binary file in input/output mode
    file.open(_fileName, ios::in | ios::out | ios::binary);
    if(!file.is_open()) {
        cout << "Couldn't open binary file. Either doesn't exist or lack permission to open it.\n";
        emptyFile = true;

        file.open(_fileName, ios::out | ios::binary); // Opens binary file in output mode.
        if(!file.is_open()){
            cout << "Failed to open file. Likely lack permissions to edit files here. Will not continue.\n";
            throw std::exception("Failure to edit file");
        }
    }

    // Stores file name in class.
    int length = lengthOfChars(_fileName) + 1; // Get length of file.
    fileName = new char[length]; // Make dynamic array for file name.
    strcpy_s(fileName, length, _fileName); // Copy _filename to class fileName
}

binaryIO::~binaryIO() {
    file.close(); // Closes file

    // Safe delete fileName;
    if(fileName != nullptr) {
        delete [] fileName;
        fileName = nullptr;
    }

    cout << "Closing file\n";
}

/// Function to append data to file.
/// @param buffer character buffer which will be written to the file.
/// @param bufferSize How many characters of buffer to be written to file. This should normally be the size of the buffer.
void binaryIO::addData(char *buffer, int bufferSize) {
    file.seekg(0,ios::end);
    file.write(buffer, bufferSize);
}


/// Function to write data to binary file.
/// @param buffer character buffer which will be written to the file.
/// @param bufferSize How many characters of buffer to be written to file. This should normally be the size of the buffer.
void binaryIO::writeData(char *buffer, int bufferSize) {
    file.write(buffer, bufferSize);
}

/// Function to write data to binary file.
/// @param bufferSize How many characters the buffer is.
/// @return buffer character buffer which will be written to the file.
char *binaryIO::readData(int &bufferSize) {
    bufferSize = getLengthOfFile();

    char *buffer = new char[bufferSize];
    file.read(buffer, bufferSize);
    return buffer;
}

/// Function to write data to binary file.
/// @return buffer character buffer which will be written to the file.
char *binaryIO::readData() {
    int bufferSize;
    return readData(bufferSize); // If there's no need for the size don't need to include
}

/// Function to get length of binary file.
/// @return int for length of file
int binaryIO::getLengthOfFile() {
    // get length of file
    file.seekg(0,ios::end); // move to the end of the file
    int fileLength = file.tellg(); // get the current position -> size of file
    file.seekg(0,ios::beg); // move to the beginning of the file (rewind)

    return fileLength;
}
