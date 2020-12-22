//
// Created by Patrick on 2020-12-02.
//

#ifndef INC_2CARS2RACING_GOAL_H
#define INC_2CARS2RACING_GOAL_H


#include "physicsHelper.h"

class goal {
private:
    double x1, x2, y1, y2; // Holds location of goal
    int playerNumber; //
public:
    goal(int playerNumber, double x1, double y1, double x2, double y2);
    goal(const goal &obj);
    ~goal();
    bool checkCollision(const physicalObject &obj);
};


#endif //INC_2CARS2RACING_GOAL_H
