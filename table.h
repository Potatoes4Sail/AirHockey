//
// Created by Patrick on 2020-11-24.
//

#ifndef INC_2CARS2RACING_TABLE_H
#define INC_2CARS2RACING_TABLE_H


#include <fstream>
#include "shapes.h"
#include "paddle.h"
#include "puck.h"
#include "goal.h"

const short int maxNumberOfLines = 25;
const short int maxNumberOfArcs = 10;
const short int maxNumberOfPucks = 10;

class table {
private:
    vector *startingPaddlePosition[2]; // Holds the default status of the two paddles
    goal *goalArray[2];
    int numberOfPlayers;
public:
    int numberOfLines;
    sLine *pLine[maxNumberOfLines];
    int numberOfArcs;
    arc *pArc[maxNumberOfArcs];
    int numberOfPucks;
    vector *puckPosArray[maxNumberOfPucks];
public:
    table(const char *fileName);
    ~table();

    int getNumberOfLines() const;
    int getNumberOfArcs() const;
    int getNumberOfPlayers() const;

    void draw();
    vector getPaddleStartingPos(unsigned int paddleNumber);
    vector getPuckStartingPos(unsigned  int puckNumber);
    goal * getGoal(int playerNumber);
private:
    void readInputFile(std::ifstream &tableFile);
    void addLine(double x1, double y1, double x2, double y2);
    void addArc(double centerX, double centerY, double radius, double theta0, double theta1);
    void addPuck(double x, double y);
    void addGoal(int playerNumber, double x1, double y1, double x2, double y2);
};

#endif //INC_2CARS2RACING_TABLE_H
