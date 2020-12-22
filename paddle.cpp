//
// Created by Patrick on 2020-11-28.
//

#include "paddle.h"
#include "camera.h"

const double PADDLE_RADIUS = 0.095;
const double PADDLE_MASS = 0.250;

paddle::paddle(const vector &startingPosition, unsigned int playerNumber) {
    // Sets puck size and radius equal to default values
    setRadius(PADDLE_RADIUS);
    object.mass = PADDLE_MASS;
    setFrictionCoefficient(0.25);
    setHeight(0.4); // Sets height of paddle

    // Sets moment of inertia of the puck
    object.momentOfInertia = 1.0 / 2.0 * object.mass * getRadius() * getRadius();

    object.position = startingPosition;
    // Sets other components of object equal to 0.
    object.velocity = 0;
    object.acceleration = 0;
    object.angularPosition = 0;
    object.angularVelocity = 0;
    object.angularAcceleration = 0;

    // Sets colour of the paddle
    paddleColour[0] = 179;
    paddleColour[1] = 3;
    paddleColour[2] = 38;

    this->playerNumber = playerNumber;
}


paddle::~paddle() {

}


/// Sets colour of paddle to values inputted from colourMatrix
void paddle::setColour(const double *colourMatrix) {
    for (int i = 0; i < 3; ++i) {
        paddleColour[i] = colourMatrix[i];
    }
}

// Draws paddle
void paddle::draw() {
    drawCircle(paddleColour, object.position, getRadius());
}
