//
// Created by Patrick on 2020-11-24.
//

#ifndef INC_2CARS2RACING_GAMEOBJECT_H
#define INC_2CARS2RACING_GAMEOBJECT_H


#include "puck.h"
#include "table.h"
#include "paddle.h"
#include "audio.h"
#include "goal.h"
#include "UI.h"

class gameObject {
    audio *pBoing;
    audio *pGoal;
    int playerScore[2]{}; // Array to hold score for each player.
    UI scoreboardUI;
    bool loadingScreen;
public:
    int numberOfPucks;
    puck *puckArray[maxNumberOfPucks]{};
    // TODO: Tidy up this part with dynamic allocation of memory for these various parts
    bool bPuckGoal[maxNumberOfPucks]{};

    table *pTable;

    paddle *paddleArray[2]{}; // Creates array for the two paddles
    goal *goalArray[2]{}; // Creates array for the two goals.
    bool goalScored;
    bool bResetTable;
public:
    gameObject();

    ~gameObject();

    void draw(); // Draws objects
    void update(); // Updates the physics of the game

    void loadTable(char *fileName);
    void clearTable(); // Removes everything in table object.
    void resetTable(); // Resets items to original position on table
    void processGoal(); // Function called when a goal is scored, will do all necessary things
    int getPoints(const int playerN); // Gets number of points that the player has.
    void setPoints(const int playerN, const int pointNumber); // Sets number of points that the player has.

    void setLoadingScreen(bool value);
private:
    void addPuck(const vector &vec); // Add a puck
    void clearPuck(); // Remove all pucks

    // Collision checking between objects
    void detectObjObjCollision(physicsHelper &obj1, physicsHelper &obj2); // Object to object
    void detectObjLineCollision(physicsHelper &obj1, sLine &lineObj); // Object to line
    void detectObjArcCollision(physicsHelper &obj1, arc &arcObj); // Object to arc

    // Apply collision equations to objects
    void applyCollisionObjObj(physicsHelper &obj1, physicsHelper &obj2); // Applies collision mechanics between objects (w/ mass)
    void applyObjLineCollision(physicsHelper &obj, sLine &lineObj); // Applies collision between object and line
    void applyObjArcCollision(physicsHelper &obj, arc &arcObj); // Applies collision between object and arc

    // Equation for calculating velocity between 2 pucks
    vector newVelocity(const physicalObject &obj1, const physicalObject &obj2);
};


#endif //INC_2CARS2RACING_GAMEOBJECT_H
