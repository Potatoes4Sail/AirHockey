//
// Created by Patrick on 2020-11-24.
//

#include "puck.h"
#include "camera.h"

// Default puck radius & mass;
double PUCK_RADIUS = 0.041275;
double PUCK_HEIGHT = 0.005;
double PUCK_MASS = 0.085;

puck::puck(const vector &startingPosition) {
    // Sets puck size and radius equal to default values
    radius = PUCK_RADIUS;
    object.mass = PUCK_MASS;
    setFrictionCoefficient(0.01);// This can use some fine tuning
    setHeight(0.05); // Sets height of puck

    // Sets moment of inertia of the puck
    object.momentOfInertia = 1.0 / 2.0 * object.mass * radius * radius;

    object.position = startingPosition;
    // Sets other components of object equal to 0.
    object.velocity = 0;
    object.acceleration = 0;
    object.angularPosition = 0;
    object.angularVelocity = 0;
    object.angularAcceleration = 0;

    // Initialize puck's colour, default is red.
    puckColour[0] = 245;
    puckColour[1] = 85;
    puckColour[2] = 25;

    // Sets the initial force equal to 0;
    force = 0;
}

puck::~puck() {

}

void puck::draw() {
    drawCircle(puckColour, object.position, radius);
}


void puck::addForce(const vector &f) {
    force += f;
}
