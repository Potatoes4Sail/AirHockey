#include "config.h"
#include "charFunctions.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

// ? Default file config name.
//const char *configFileName = "./assets/config.txt"; // ! Eventually return to this file location.
const char *configFileName = "./config.txt";

config::config() {
    ifstream configFile;
    configFile.open(configFileName);

    // Generates config file.
    if((!configFile.is_open())) {
        configFile.close();
        cout << "WARNING! Config file is not found. Will try to generate one.\n";
        defaultInitialization();
        configFile.open(configFileName);
    }

    //  Verifies that the file is open, and then will try to read it.
    if(configFile.is_open()) {
        readInputFileStream(configFile);
    } else {
        cout << "Failed to read config file after attempting to create it. Please verify that the file actually exists.\n";
        throw invalid_argument("Cannot open config file.");
    }
    configFile.close();
}

void config::readInputFileStream(ifstream &configFile) {
    unsigned int lineCount;
    getLengthOfFile(configFile, lineCount);

    numberOfConfigValues = lineCount;

    configNames = new char *[numberOfConfigValues];
    configCharValues = new char *[numberOfConfigValues];



    for (int i = 0; i < numberOfConfigValues; ++i) {
        constexpr int maxLengthOfVariableName = 128;

        // Used to enter values into the variable name array.
        char inputCharArray[maxLengthOfVariableName]{};
        configFile >> inputCharArray;

        int lengthOfString = lengthOfChars(inputCharArray);
        configNames[i] = new char[lengthOfString + 1]; // Creates new array to store variable name.

        int nOfChars;
        for (nOfChars = 0; nOfChars < lengthOfString; ++nOfChars) {
            configNames[i][nOfChars] = inputCharArray[nOfChars];
        }
        configNames[i][nOfChars] = '\0';
        // Stores variable name in character array


        // Inputs variable value of into temporary character array.
        configFile >> inputCharArray;

        lengthOfString = lengthOfChars(inputCharArray);
        configCharValues[i] = new char[lengthOfString + 1]; // Creates new array to store name.

        nOfChars;
        for (nOfChars = 0; nOfChars < lengthOfString; ++nOfChars) {
            configCharValues[i][nOfChars] = inputCharArray[nOfChars];
        }
        configCharValues[i][nOfChars] = '\0';
        // Stores name in character array
    }
}

config::~config() {
    // Cleans up dynamic arrays when removing object
    for (int i = 0; i < numberOfConfigValues; ++i) {
        delete[] configNames[i];
        delete[] configCharValues[i];
    }
    if(configNames != nullptr) {
        delete[] configNames;
        configNames = nullptr;
    }

    if(configCharValues != nullptr) {
        delete[] configCharValues;
        configCharValues = nullptr;
    }
}

void config::print() const{
    for (int i = 0; i < numberOfConfigValues; ++i) {
        cout << configNames[i] << ":\t" << configCharValues[i] << "\n";
    }
}

/// Function returns a char array for a variable.
/// @param findConfigName - Name of variable to get value of
/// @return value stored in config file
char *config::getCharValue(const char *findConfigName) const {
    for (int i = 0; i < numberOfConfigValues; ++i) {
        if(compareChars(configNames[i], findConfigName) == 1) return configCharValues[i];
    }
    cout << "\nFailed to find value in config. Program will probably break now! :(\n";
    return nullptr;
}

/// Function returns a double value for a variable.
/// @param findConfigName - Name of variable to get value of
/// @return value stored in config file
double config::getDoubleValue(const char *findConfigName) const { //TODO, call by reference and make function bool?
    for (int i = 0; i < numberOfConfigValues; ++i) {
        if(compareChars(configNames[i], findConfigName) == 1){
            return atof(configCharValues[i]);
        }
    }
    return -1;
}

/// Function returns a int value for a variable.
/// @param findConfigName - Name of variable to get value of
/// @return value stored in config file
int config::getIntValue(const char *findConfigName) const {
    double variableValue = getDoubleValue(findConfigName);
    return (int) std::round(variableValue); // Rounds value and returns this
}


/// Function returns a bool value for a variable.
/// @param findConfigName - Name of variable to get value of
/// @return value stored in config file
bool config::getBoolValue(const char *findConfigName) const {
    double variableValue = getDoubleValue(findConfigName);
    double minimumTrueValue = 1e-7; // Sets a very small value to avoid having rounding errors make a 0 value become non-zero.
    return (variableValue > minimumTrueValue); // Returns true if value is > 1e-7 (ie. it's not 0)
}

bool config::setValue(const char *configName, char *value) {
    for (int i = 0; i < numberOfConfigValues; ++i) {
        if(compareChars(configNames[i], configName) == 1) {
            delete[] configCharValues[i]; // Removes old value from memory

            int length = lengthOfChars(value) + 1; // Gets the new length
            configCharValues[i] = new char[length];
            for (int currentChar = 0; currentChar < length; ++currentChar) {
                configCharValues[i][currentChar] = value[currentChar];
            }
            return true;
        }
    }
    return false;
}


bool config::defaultInitialization() {
    ofstream outputFile;
    outputFile.open(configFileName);
    if(!outputFile.is_open()) return false;

    outputFile << "WINDOW_WIDTH 1200\n"
                  "WINDOW_HEIGHT 800\n"
                  "NETWORK_GAME 0\n"
                  "NUMBER_OF_PLAYERS 2\n"
                  "GAMEPAD_ENABLED 0\n"
                  "HOST_IP 192.168.2.90\n"
                  "CLIENT_IP 192.168.2.21\n"
                  "TABLE_FILE table.txt\n"
                  "AI_LEVEL 0\n"
                  "PLAYER_NAME1 JoesPizza\n"
                  "PLAYER_NAME2 PineapplePizza\n"
                  "IPv4 1\n"
                  "MUTE 0";
    outputFile.close();
    return true;
}

void config::saveFile() {
    // Opens file to write to
    ofstream outputFile;
    outputFile.open(configFileName);

    if(outputFile.is_open()) {
        for (int i = 0; i < numberOfConfigValues; ++i) {
            outputFile << this->configNames[i] << " " << this->configCharValues[i]; // Outputs each line of the file
            if(i < numberOfConfigValues - 1) outputFile << "\n"; // Output a new line for each line except last line.
        }
    }
}


config *getConfig() {
    config *pConfig = nullptr;
    try {
        pConfig = new config;
    } catch (invalid_argument &e) {
        cerr << e.what() << endl;
    }
    return pConfig;
}