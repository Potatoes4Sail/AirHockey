//
// Created by Patrick on 2020-12-05.
//

#ifndef INC_2CARS2RACING_BINARYIO_H
#define INC_2CARS2RACING_BINARYIO_H

#include <iostream>
#include <fstream>

class binaryIO {
    char *fileName; // Stores file name (for reasons I'm not sure why tbh)
    std::fstream file; // File object
    bool emptyFile; // Variable to store if file is empty when opened. If file is empty when opened it cannot be read from.
public:
    binaryIO(const char *_fileName);
    ~binaryIO();
    void addData(char *buffer, int bufferSize);
    void writeData(char *buffer, int bufferSize);
    char *readData(int &bufferSize);
    char *readData();
    int getLengthOfFile();
};
#endif //INC_2CARS2RACING_BINARYIO_H
