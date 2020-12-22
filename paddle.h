//
// Created by Patrick on 2020-11-28.
//

#ifndef INC_2CARS2RACING_PADDLE_H
#define INC_2CARS2RACING_PADDLE_H


#include "physicsHelper.h"

class paddle : public physicsHelper {
    double paddleColour[3]; // Colour of paddle to be drawn
    unsigned int playerNumber; // Holds player number?
public:
    // Constructor & destructors
    paddle(const vector &startingPosition, unsigned int playerNumber);
    ~paddle();

    void setColour(const double colourMatrix[3]);
    void draw(); // Draws paddle.
};


#endif //INC_2CARS2RACING_PADDLE_H
