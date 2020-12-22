//
// Created by Patrick on 2020-11-03.
//

#ifndef SIDE_RACER_PHYSICSHELPER_H
#define SIDE_RACER_PHYSICSHELPER_H
#include "vector.h"
#include "physicalObject.h"

class physicsHelper {
private:
    double timeSinceLastCall; ///< Used to calculate the amount the object should move between calls.
public:
    double frictionCoefficient; ///< Stores value for friction
    vector force; ///< Stores value for any forces acting on object
    double torque; ///< Stores value for any torques acting on object
    double radius; ///< Radius of part, defaults to 1
    double height; ///< Height of part, defaults to 1
public:
    physicalObject object;
public:
    physicsHelper();
    ~physicsHelper();

    void update(); // Updates object with forces from drag
    void propagate(const vector &force, const double &torque = 0); // Moves object
public:

    vector getPosition();
    void setPosition(const vector &position);

    vector getVelocity();
    void setVelocity(const vector &velocity);

    double getMass();

    const physicalObject &getObject() const;
    physicsHelper* getPhysicsHelper();

    // Gets and setters for objects.
    double getRadius() const;
    void setRadius(double _radius);

    // get/set friction coefficient
    double getFrictionCoefficient() const;
    void setFrictionCoefficient(double frictionCoefficient);

    vector getForce() const;
    void setForce(vector &force);

    double getTorque() const;
    void setTorque(double torque);

    double getHeight() const;
    void setHeight(double height);
};


#endif //SIDE_RACER_PHYSICSHELPER_H


