//
// Created by Patrick on 2020-11-03.
//

#ifndef SIDE_RACER_PHYSICALOBJECT_H
#define SIDE_RACER_PHYSICALOBJECT_H

#include "vector.h"

struct physicalObject {
    vector position, velocity, acceleration;
    double angularPosition, angularVelocity, angularAcceleration;

    double momentOfInertia;
    double mass;
};
#endif //SIDE_RACER_PHYSICALOBJECT_H
