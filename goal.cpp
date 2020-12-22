//
// Created by Patrick on 2020-12-02.
//

#include "goal.h"

goal::goal(int playerNumber, double x1, double y1, double x2, double y2) {
    this->playerNumber = playerNumber;

    if (x1 < x2) {
        this->x1 = x1;
        this->x2 = x2;
    } else {
        this->x1 = x2;
        this->x2 = x1;
    }

    if (y1 < y2) {
        this->y1 = y1;
        this->y2 = y2;
    } else {
        this->y1 = y2;
        this->y2 = y1;
    }

    // Stores player number and location for goal.
}

// Copy constructor

goal::goal(const goal &obj) {
    playerNumber = obj.playerNumber;
    x1 = obj.x1;
    x2 = obj.x2;
    y1=obj.y1;
    y2=obj.y2;
}


goal::~goal() {

}

bool goal::checkCollision(const physicalObject &obj) {
    if((x1 < obj.position.x &&  obj.position.x < x2) && (y1 < obj.position.y && obj.position.y < y2)){
        return true;
    }
    return false;
}
