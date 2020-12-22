#include "physicsHelper.h"
#include "2D_graphics/timer.h"


using namespace std;
physicsHelper::physicsHelper(){

    // Sets all the vectors to default values. For most this is equal to 0.
    object.position = 0;
    object.velocity = 0;
    object.acceleration = 0;
    object.angularPosition = 0;
    object.angularVelocity = 0;
    object.angularAcceleration = 0;

    object.mass = 1; // Don't want to set this to 0 as later it will be used to divide and want to avoid that error.
    object.momentOfInertia = object.mass * 10; // This value should be set later on, however this just ensures that the it has a somewhat reasonable initial value

    // Sets other physical attributes to 0
    force = 0;
    torque = 0;
    radius = 0;
    height = 0;
    frictionCoefficient = 0;

    // Sets time to 0
    timeSinceLastCall = 0;
}

physicsHelper::~physicsHelper(){
    //TODO: Remove any dynamic items here.
}

void physicsHelper::update() {
    if(norm(getVelocity()) > 1e-7) {
        // Calculated friction on object
        force -= unit(object.velocity) * object.mass * frictionCoefficient * 9.81;

        // Calculate drag on object
        const double densityOfAir = 1.225;
        const double dragCoefficient = 0.47;
        force -= unit(object.velocity) * (1.0 / 2.0) * densityOfAir * norm(object.velocity) * norm(object.velocity) *
                 dragCoefficient * (radius * 2 * height);
    } else {
        setVelocity(vector(0,0));
    }
    // Propagate object
    propagate(force);
    force = 0;
}

void physicsHelper::propagate(const vector &force, const double &torque) {
    auto currentTimeFunctionCalled = high_resolution_time();
    double deltaT = currentTimeFunctionCalled - timeSinceLastCall;

    const double maxTimeStep = 0.025;
    // deltaT = ((deltaT > maxTimeStep) ? maxTimeStep : deltaT); // Limits max time step to 0.02 seconds
    deltaT = 0.001; // Sets to a fixed time as program will be looped.
    // TODO: This method of handling the time interval causes issues with different loops.

    // ? Propagation code

    // Get linear acceleration of object.
    object.acceleration = force / object.mass;

    // Propagate linear motion
    object.velocity += object.acceleration * deltaT;
    object.position += object.velocity * deltaT;

/*

    // Get angular acceleration of object.
    object.angularAcceleration = torque / object.momentOfInertia;

    // Propagate angular motion
    object.angularVelocity += object.angularAcceleration * deltaT;
    object.angularPosition += object.angularVelocity * deltaT;

*/

    timeSinceLastCall = currentTimeFunctionCalled;
}

vector physicsHelper::getPosition() {
    vector pos = object.position;
    return pos;
}

void physicsHelper::setPosition(const vector &position) {
    object.position = position;
}

vector physicsHelper::getVelocity() {
    vector velocity = object.velocity;
    return velocity;
}

void physicsHelper::setVelocity(const vector &velocity) {
    object.velocity = velocity;
}

double physicsHelper::getMass() {
    return object.mass;
}

double physicsHelper::getRadius() const {
    return radius;
}

void physicsHelper::setRadius(double _radius) {
    physicsHelper::radius = _radius;
}

physicsHelper* physicsHelper::getPhysicsHelper() {
    return this;
}

const physicalObject &physicsHelper::getObject() const {
    return object;
}

double physicsHelper::getFrictionCoefficient() const {
    return frictionCoefficient;
}

void physicsHelper::setFrictionCoefficient(double frictionCoefficient) {
    physicsHelper::frictionCoefficient = frictionCoefficient;
}

vector physicsHelper::getForce() const {
    return force;
}

void physicsHelper::setForce(vector &force) {
    physicsHelper::force = force;
}

double physicsHelper::getTorque() const {
    return torque;
}

void physicsHelper::setTorque(double torque) {
    physicsHelper::torque = torque;
}

double physicsHelper::getHeight() const {
    return height;
}

void physicsHelper::setHeight(double height) {
    physicsHelper::height = height;
}