//
// Created by Patrick on 2020-11-24.
//

#include "table.h"
#include "charFunctions.h"
#include "customMath.h"

using namespace std;

table::table(const char *fileName) {
    // Sets up necessary things for variables first.

    // Sets value of lines to 0
    numberOfLines = 0;
    numberOfArcs = 0;
    numberOfPucks = 0;
    numberOfPlayers = 0;

    // Sets pLine to null to avoid errors in deleting
    for (int i = 0; i < maxNumberOfLines; ++i) {
        pLine[i] = nullptr;
    }

    // Sets pArc to null to avoid errors in deleting
    for (int i = 0; i < maxNumberOfArcs; ++i) {
        pArc[i] = nullptr;
    }

    // Create dynamic array for paddle positions;
    for (int i = 0; i < 2; ++i) {
        startingPaddlePosition[i] = new vector(0, 0);
    }

    // Create dynamic array for goals
    for (int i = 0; i < 2; ++i) {
        goalArray[i] = nullptr;
    }

    // Sets pArc to null to avoid errors in deleting
    for (int i = 0; i < maxNumberOfPucks; ++i) {
        puckPosArray[i] = nullptr;
    }

    // Read file once needed variables are created.
    ifstream tableFile;
    tableFile.open(fileName);

    if (tableFile.is_open()) {
        readInputFile(tableFile);
    }
    tableFile.close();

}

table::~table() {
    //  Deletes all the pLines
    for (int i = 0; i < maxNumberOfLines; ++i) {
        if(pLine[i] != nullptr){
            delete pLine[i];
            pLine[i] = nullptr;
        }
    }

    // Sets pArc to null to avoid errors in deleting
    for (int i = 0; i < maxNumberOfArcs; ++i) {
        if(pArc[i] != nullptr){
            delete pArc[i];
            pArc[i] = nullptr;
        }
    }

    // Sets puck to null to avoid errors in deleting
    for (int i = 0; i < maxNumberOfPucks; ++i) {
        if(puckPosArray[i] != nullptr){
            delete puckPosArray[i];
            puckPosArray[i] = nullptr;
        }
    }

    // Checks if startingPaddlePosition was set and if so delete
    for (int i = 0; i < 2; ++i) {
        if(startingPaddlePosition[i] != nullptr) {
            delete startingPaddlePosition[i];
            startingPaddlePosition[i] = nullptr;
        }
    }

    // Checks if goalArray was set and if so delete
    for (int i = 0; i < 2; ++i) {
        if(goalArray[i] != nullptr) {
            delete goalArray[i];
            goalArray[i] = nullptr;
        }
    }
}

void table::readInputFile(std::ifstream &tableFile) {
    unsigned int lineCount;
    getLengthOfFile(tableFile, lineCount);

//    cout << "matches of line = " << countMatches(tableFile, "line") << "\n";
//    cout << "matches of arc = " << countMatches(tableFile, "arc") << "\n";

    for (int i = 0; i < lineCount; ++i) {
        char type[10];
        tableFile >> type;
        // Check if value is line
        if (compareChars(type, "line")) {
            // Value in format of:
            // line startX startY endX endY
            double values[4];
            for (double &value : values) { // if it is populate array with values and create a line
                tableFile >> value;
            }
            addLine(values[0], values[1], values[2], values[3]);
        } else if (compareChars(type, "arc")) {    // Check if value is arc
            // Value in format of:
            // arc centerX centerY radius startAngle endAngle
            //      - note: start and end angles are inputted in degrees
            double values[5];
            for (double &value : values) { // if it is populate array with values and create an arc
                tableFile >> value;
            }
            addArc(values[0], values[1], values[2], toRadians(values[3]), toRadians(values[4]));
        } else if (compareChars(type, "paddle")) {
            // Value in format of:
            // paddle playerNumber x y
            double values[3];
            for (double &value : values) { // if it is populate array with values and create an paddle
                tableFile >> value;
            }
            int playerNumber = (int) values[0];
            if (playerNumber <= 1) {
                *startingPaddlePosition[playerNumber] = vector(values[1], values[2]);
            }
            // Sets x & y to player n;
            // As two players values can be 0 or 1;
        } else if (compareChars(type, "puck")) {
            // Value in format of:
            // puck x y
            double values[2];
            for (double &value : values) { // if it is populate array with values and create an puck
                tableFile >> value;
            }
            addPuck(values[0], values[1]);
        } else if (compareChars(type, "goal")) {
            // Value in format of:
            // goal playerNumber x1 y1 x2 y2
            double values[5];
            for (double &value : values) { // if it is populate array with values and create an puck
                tableFile >> value;
            }
            addGoal((int) values[0], values[1], values[2], values[3], values[4]);
        }
        // TODO: Implement additional shapes if necessary
    }
}

void table::addLine(double x1, double y1, double x2, double y2) {
    if (numberOfLines < maxNumberOfLines) {
        pLine[numberOfLines] = new sLine(x1, y1, x2, y2);
        numberOfLines++;
    } else {
        std::cout << "WARNING: Cannot make additional line objects. Reached limit. \n";
    }
}

void table::draw() {
    // Draws all lines
    if (pLine[0] != nullptr) {
        for (int lineN = 0; lineN < numberOfLines; ++lineN) {
            pLine[lineN]->draw();
        }
    }

    // draws all arcs
    if (pArc[0] != nullptr) {
        for (int arcN = 0; arcN < numberOfArcs; ++arcN) {
            pArc[arcN]->draw();
        }
    }
}

int table::getNumberOfLines() const {
    return numberOfLines;
}

void table::addArc(double centerX, double centerY, double radius, double theta0, double theta1) {
    if (numberOfArcs < maxNumberOfArcs) {
        pArc[numberOfArcs] = new arc(centerX, centerY, radius, theta0, theta1);
        numberOfArcs++;
    } else {
        std::cout << "WARNING: Cannot make additional arc objects. Reached limit. \n";
    }
}

int table::getNumberOfArcs() const {
    return numberOfArcs;
}

int table::getNumberOfPlayers() const {
    return numberOfPlayers;
}


vector table::getPaddleStartingPos(unsigned int paddleNumber) {
    // return default position of paddle
    return *startingPaddlePosition[paddleNumber];
}

void table::addPuck(double x, double y) {
    if(numberOfPucks < maxNumberOfPucks){
        puckPosArray[numberOfPucks] = new vector(x, y);
        numberOfPucks++;
    } else {
        std::cout << "WARNING: Cannot make additional puck objects. Reached limit. \n";
    }
}

vector table::getPuckStartingPos(unsigned int puckNumber) {
    return *puckPosArray[puckNumber];
}

goal *table::getGoal(int playerNumber) {
    return goalArray[playerNumber];
}

void table::addGoal(int playerNumber, double x1, double y1, double x2, double y2) {
    goalArray[playerNumber] = new goal(playerNumber, x1, y1, x2, y2);
    numberOfPlayers++; // Increments number of players.
}