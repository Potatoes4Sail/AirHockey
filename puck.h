//
// Created by Patrick on 2020-11-24.
//

#ifndef INC_2CARS2RACING_PUCK_H
#define INC_2CARS2RACING_PUCK_H

#include "physicsHelper.h"

//TODO: Make this protected
class puck : public physicsHelper {
    double puckColour[3];
    vector force;
public:
    puck(const vector &startingPosition = vector(100,100));
    ~puck();
    void draw();

    void addForce(const vector &f);
};


#endif //INC_2CARS2RACING_PUCK_H
