//
// Created by Patrick on 2020-11-24.
//

#include <Windows.h>

#include "gameObject.h"
#include <iostream>
#include "gameInfo.h"

extern gameInfo gInfo;

gameObject::gameObject() {
    // Objects to hold sounds files for game.
    pBoing = new audio((char *) "assets/audio/boing.wav");
    pGoal = new audio((char *) "assets/audio/goal.wav");

    numberOfPucks = 0;

    for (int i = 0; i < maxNumberOfPucks; ++i) {
        puckArray[i] = nullptr;
    }

    // Sets table pointer to nullptr
    pTable = nullptr;

    // Sets default paddle array to nullptrs
    for(int i = 0; i < 2; i++) {
        paddleArray[i] = nullptr;
    }

    // Sets default goal array to nullptrs
    for(int i = 0; i < 2; i++) {
        goalArray[i] = nullptr;
    }

    // Checked to see if a goal was scored
    goalScored = false;
    bResetTable = false;

    // Sets all the goals to false.
    for (int puckNumber = 0; puckNumber < maxNumberOfPucks; ++puckNumber) {
        bPuckGoal[puckNumber] = false;
    }

    // Sets points to 0 for each player initially.
    for (int i = 0; i < 2; ++i) {
        playerScore[i] = 0;
    }

    // Creates menu object.
    scoreboardUI.clear();
    vector scoreCenter(200, 800);
    scoreboardUI.addText(vector(50, 75), gInfo.player0Name, 1);
    scoreboardUI.addText(vector(1000, 75), gInfo.player1Name, 1);

    scoreboardUI.addText(scoreCenter, "Score = ", 1.5); // used to draw "score"
    scoreboardUI.addText(scoreCenter + vector(300, -14), "0 - 0", 1); // Draws text for the actual value of the score

    loadingScreen = false;
}

gameObject::~gameObject() {
    // Remove all puck objects
    for (int i = 0; i < maxNumberOfPucks; ++i) {
        if (puckArray[i] != nullptr) {
            delete puckArray[i];
            puckArray[i] = nullptr;
        }
    }

    if(pTable != nullptr){
        delete pTable;
        pTable = nullptr;

        for (int i = 0; i < 2; ++i) { // Deletes paddle array pointers
            if(paddleArray[i] != nullptr){
                delete paddleArray[i];
                paddleArray[i] = nullptr;
            }
        }

        for (int i = 0; i < 2; ++i) { // Deletes goal array pointers
            if (goalArray[i] != nullptr) {
                delete goalArray[i];
                goalArray[i] = nullptr;
            }
        }
    }
    // Delete audio objects
    delete pBoing;
    delete pGoal;
}

void gameObject::addPuck(const vector &vec) {
    // Add a puck to the object
    if (numberOfPucks < maxNumberOfPucks) {
        puckArray[numberOfPucks] = new puck(vec);
        numberOfPucks++;
    } else {
        std::cout << "WARNING: Cannot make additional button objects. Reached limit. \n";
    }
}

void gameObject::clearPuck() {
    for (int i = 0; i < numberOfPucks; ++i) {
        if (puckArray[i] != nullptr) {
            delete puckArray[i];
            puckArray[i] = nullptr;
        }
    }
    numberOfPucks = 0;
}

void gameObject::draw() {
    // Draws all the pucks
    for (int i = 0; i < numberOfPucks; ++i) {
        puckArray[i]->draw();
    }

    // Draws table
    if(pTable != nullptr) pTable->draw();

    // Draws paddles
    for(int i = 0; i < 2; i++) {
        if (paddleArray[i] != nullptr) paddleArray[i]->draw();
    }

    // Updates the scoreboard to show the player scores & not on loading screen
    if (pTable != nullptr  && !loadingScreen) {
        char bufferText[50];
        sprintf(bufferText, "%d - %d", playerScore[0], playerScore[1]); // Formats buffer text
        scoreboardUI.updateLastText(bufferText);

        // Draws the scoreboard
        scoreboardUI.draw();
    }
}


void gameObject::update() {
    int timeSteps = 15;
    for (int t = 0; t < timeSteps; ++t) {
        bResetTable = false; // Resets the table to not be being reset.

        // Checks if a puck is in a goal. If so it should reload from locations of items from file.

        // Look at collision for pucks
        int numberOfPlayers = pTable->getNumberOfPlayers();
        for (int i = 0; i < numberOfPucks; ++i) {
            for (int j = i + 1; j < numberOfPucks; ++j) {
                // Checks if there are any collisions (between pucks)
                detectObjObjCollision(*puckArray[i]->getPhysicsHelper(), *puckArray[j]->getPhysicsHelper());
            }

            // Don't check if there is collision with the table if the table doesn't exist
            if (pTable != nullptr) {
                // Check if there are any collisions with the lines
                for (int lineN = 0; lineN < pTable->getNumberOfLines(); ++lineN) {
                    detectObjLineCollision(*puckArray[i]->getPhysicsHelper(), *pTable->pLine[lineN]);
                }

                // Check if there are any collisions with the arcs
                for (int arcN = 0; arcN < pTable->getNumberOfArcs(); ++arcN) {
                    detectObjArcCollision(*puckArray[i]->getPhysicsHelper(), *pTable->pArc[arcN]);
                }

                // Check if there are any collisions with the paddles & pucks
                for (int paddleN = 0; paddleN < numberOfPlayers; ++paddleN) {
                    detectObjObjCollision(*paddleArray[paddleN]->getPhysicsHelper(), *puckArray[i]->getPhysicsHelper());
                }
            }
        }

        if (pTable != nullptr) { // ie. table exists
            // Looks at collision for paddles
            for (int paddleN = 0; paddleN < numberOfPlayers; ++paddleN) {
                // Check if there are any collisions with the lines
                for (int lineN = 0; lineN < pTable->getNumberOfLines(); ++lineN) {
                    detectObjLineCollision(*paddleArray[paddleN]->getPhysicsHelper(), *pTable->pLine[lineN]);
                }

                // Check if there are any collisions with the arcs
                for (int arcN = 0; arcN < pTable->getNumberOfArcs(); ++arcN) {
                    detectObjArcCollision(*paddleArray[paddleN]->getPhysicsHelper(), *pTable->pArc[arcN]);
                }

                // Checks if there are any collisions (between paddles)
                for (int paddleM = paddleN + 1; paddleM < numberOfPlayers; ++paddleM) {
                    detectObjObjCollision(*paddleArray[paddleN]->getPhysicsHelper(),
                                          *paddleArray[paddleM]->getPhysicsHelper());
                }
            }

            // Checks if a goal has been scored.
            for (int goalN = 0; goalN < numberOfPlayers; ++goalN) {
                for (int puckNumber = 0; puckNumber < numberOfPucks; ++puckNumber) {
                    // Checks if there is a puck in the goal and that the goal hasn't been processed for that puck yet.
                    if (goalArray[goalN]->checkCollision(puckArray[puckNumber]->getObject()) &&
                        !bPuckGoal[puckNumber]) {
                        bPuckGoal[puckNumber] = true;
                        int playerScoredOn = !goalN; // The score should go to the opposite person to who was scored on.
                        playerScore[playerScoredOn]++; // Increment score
                        goalScored = true;
                    }
                }
            }
            // Updates all the paddles
            for (int i = 0; i < numberOfPlayers; ++i) {
                paddleArray[i]->update();
            }

        }

        // Updates all the pucks
        for (int i = 0; i < numberOfPucks; ++i) {
            puckArray[i]->update();
        }


        // checks if location of pucks is outside table & not on loading screen
        if (pTable != nullptr && !loadingScreen) {
            for (int i = 0; i < numberOfPucks; ++i) {
                vector pos = puckArray[i]->getPosition();
                if (pos.x < -2.5 || pos.x > 2.5 || pos.y < -numberOfPlayers || pos.y > numberOfPlayers) {
                    std::cout << "Position is ";
                    printVector(pos);
                    std::cout << "Velocity is ";
                    printVector(puckArray[i]->getVelocity());

                    std::cout << "You hit it sooo hard it left the table!\nResetting!\n";
                    Sleep(1000);
                    resetTable();
                }
            }
        }
    }
}
//
//// Remove these
//// If two pucks are close enough to each other, then apply collision formula
//void gameObject::checkPuckPuckCollision(int puck1, int puck2) {
//    if (norm(puckArray[puck1]->getPosition() - puckArray[puck2]->getPosition()) <
//        (puckArray[puck1]->getRadius() + puckArray[puck2]->getRadius())) {
//        if(!bPuckPuckCollision[puck1][puck2]) { // Checks if a collisions has already been applied to this object.
//            applyCollisionPuckPuck(*puckArray[puck1], *puckArray[puck2]);
//            bPuckPuckCollision[puck1][puck2] = true;
//        }
//    } else {
//        bPuckPuckCollision[puck1][puck2] = false;
//    }
//}
//
//// Calculates collision between two objects
//void gameObject::applyCollisionPuckPuck(puck &p1, puck &p2) {
//    vector newVelocityP1, newVelocityP2;
//    newVelocityP1 = newVelocity(p1.object, p2.object);
//    newVelocityP2 = newVelocity(p2.object, p1.object);
//    p1.setVelocity(newVelocityP1);
//    p2.setVelocity(newVelocityP2);
//}
//
//void gameObject::detectPuckLineCollision(int puckN, int lineN) {
//    sLine lineObj = *pTable->pLine[lineN];
//    puck puckObj = *puckArray[puckN];
//    const vector &edgeToPuck = lineObj.vectorToPoint(puckObj.getPosition());
//    if (norm(edgeToPuck) < puckObj.getRadius()) {
//        // If puck is moving away from the wall, don't change velocity
//        if (dotProduct(puckObj.getVelocity(), edgeToPuck) / norm(edgeToPuck) < 0) {
//            applyCollisionPuckLine(puckObj, lineObj);
//        }
//    }
//}
//
//void gameObject::applyCollisionPuckLine(puck &p1, const sLine &l) {
//    // Parallel component to velocity doesn't change.
//    vector vParallel = projection(p1.getVelocity(), l.getLineVector());
//
//    // Perpendicular velocity should switch directions.
//    vector vPerpendicular = -1 * (p1.getVelocity() - vParallel);
//    p1.setVelocity(vParallel + vPerpendicular);
//}
//
//void gameObject::detectPuckArcCollision(puck *pPuck, arc *pArc) {
//    const vector &edgeToPuck = pArc->vectorToPoint(pPuck->getPosition());
//     if(norm(edgeToPuck) < pPuck->getRadius()){ // Is closer to wall than radius of puck. ie you're touching it
//        if(dotProduct(pPuck->getVelocity(), edgeToPuck) / norm(edgeToPuck) < 0){
//            // If puck is moving towards circle then apply collision.
//            applyCollisionPuckArc(*pPuck, pArc, edgeToPuck);
//        }
//    }
//}
//
//void gameObject::applyCollisionPuckArc(puck &p1, const arc *a, const vector &edgeToPuck) {
//    // Parallel component to velocity doesn't change.
//    vector vPerpendicular = 1 * projection(p1.getVelocity(), edgeToPuck/norm(edgeToPuck));
//
//    // Perpendicular velocity should switch directions.
//    vector vParallel = (p1.getVelocity() - vPerpendicular);
//    p1.setVelocity(vParallel - vPerpendicular);
//}

// sets table to table from file "fileName"
void gameObject::loadTable(char *fileName) {
    char bufferText[128];
    sprintf(bufferText, "assets/tables/%s", fileName);
    // Used to join file name with directory, allows for simpler calling to set value.

    pTable = new table(bufferText);
    // Initialize paddles
    for (int playerNumber = 0; playerNumber < pTable->getNumberOfPlayers(); ++playerNumber) {
        vector startingPos = pTable->getPaddleStartingPos(playerNumber);
        paddleArray[playerNumber] = new paddle(startingPos, playerNumber);
        
        goalArray[playerNumber] = new goal(*pTable->getGoal(playerNumber));
//        goalArray[playerNumber] = pTable->getGoal(playerNumber);
    }

    for (int i = 0; i < pTable->numberOfPucks; ++i) {
        addPuck(pTable->getPuckStartingPos(i));
    }
}

void gameObject::clearTable() {
    if(pTable != nullptr){ // Deletes table pointer
        delete pTable;
        pTable = nullptr;

        for (int i = 0; i < 2; ++i) { // Deletes paddle array pointers
            if(paddleArray[i] != nullptr){
                delete  paddleArray[i];
                paddleArray[i] = nullptr;
            }
        }

        for (int i = 0; i < 2; ++i) { // Deletes goal array pointers
            if(goalArray[i] != nullptr){
                delete  goalArray[i];
                goalArray[i] = nullptr;
            }
        }
    }
}


void gameObject::detectObjObjCollision(physicsHelper &obj1, physicsHelper &obj2) {
    if(distance(obj1.getPosition(),obj2.getPosition()) < (obj1.getRadius() + obj2.getRadius())){
        // if moving towards each other:
        vector relativeVelocity = obj2.getVelocity() - obj1.getVelocity();
        vector relativePosition = obj2.getPosition() - obj1.getPosition();
        if (dotProduct(relativeVelocity, relativePosition) < 0) {
            pBoing->play();
            applyCollisionObjObj(obj1, obj2);
//            std::cout << "Collision happened here \n";
        } else {
//            std::cout << "Moving away here\n";
        }
    }
}

void gameObject::detectObjLineCollision(physicsHelper &obj1, sLine &lineObj) {
    const vector &edgeToPuck = lineObj.vectorToPoint(obj1.getPosition());
    if (norm(edgeToPuck) < obj1.getRadius()) {
        // If object is moving away from the wall, don't change velocity
        if (dotProduct(obj1.getVelocity(), edgeToPuck) < 0) {
            applyObjLineCollision(obj1, lineObj);
        }
    }

}

void gameObject::detectObjArcCollision(physicsHelper &obj1, arc &arcObj) {
    const vector &edgeToPuck = arcObj.vectorToPoint(obj1.getPosition());
    if(norm(edgeToPuck) < obj1.getRadius()){ // Is closer to wall than radius of object. ie you're touching it
        if(dotProduct(obj1.getVelocity(), edgeToPuck) < 0){
            // If object is moving towards arc then apply collision.
            applyObjArcCollision(obj1, arcObj);
        }
    }
}

void gameObject::applyCollisionObjObj(physicsHelper &obj1, physicsHelper &obj2) {
    vector newVelocityObj1, newVelocityObj2;
    newVelocityObj1 = newVelocity(obj1.getObject(), obj2.getObject());
    newVelocityObj2 = newVelocity(obj2.getObject(), obj1.getObject());
    obj1.setVelocity(newVelocityObj1);
    obj2.setVelocity(newVelocityObj2);
}


void gameObject::applyObjLineCollision(physicsHelper &obj, sLine &lineObj) {
    // Parallel component to velocity doesn't change.
    vector vParallel = projection(obj.getVelocity(), lineObj.getLineVector());

    // Perpendicular velocity should switch directions.
    vector vPerpendicular = -1 * (obj.getVelocity() - vParallel);
    obj.setVelocity(vParallel + vPerpendicular);
}

void gameObject::applyObjArcCollision(physicsHelper &obj, arc &arcObj) {
    const vector &edgeToPuck = arcObj.vectorToPoint(obj.getPosition());

    // Parallel component to velocity doesn't change.
    vector vPerpendicular = projection(obj.getVelocity(), edgeToPuck/norm(edgeToPuck));

    // Perpendicular velocity should switch directions.
    vector vParallel = (obj.getVelocity() - vPerpendicular);
    obj.setVelocity(vParallel - vPerpendicular);
}

// Calculates velocity in 2 dimensions
vector gameObject::newVelocity(const physicalObject &obj1, const physicalObject &obj2) {
    return obj1.velocity - (obj1.position - obj2.position) * (2 * obj2.mass) / (obj1.mass + obj2.mass) *
                           ((dotProduct(obj1.velocity - obj2.velocity, obj1.position - obj2.position)) /
                            (norm(obj1.position - obj2.position) * norm(obj1.position - obj2.position)));
}

void gameObject::resetTable() {
    bResetTable = true;
    // Resets paddle location
    for (int paddlesN = 0; paddlesN < 2; ++paddlesN) {
        vector startingPosition = pTable->getPaddleStartingPos(paddlesN);
        paddleArray[paddlesN]->setPosition(startingPosition); // Sets position to starting position
        paddleArray[paddlesN]->setVelocity(vector(0,0)); // Sets velocity to 0
    }

    // Resets puck locations;

    for (int puckN = 0; puckN < numberOfPucks; ++puckN) {
        vector startingPosition = pTable->getPuckStartingPos(puckN);
        puckArray[puckN]->setPosition(startingPosition); // Sets position of puck to starting position
        puckArray[puckN]->setVelocity(vector(0,0)); // Sets velocity of puck to 0
    }
}

/// Function which handles goal processing
/// @return - false if no winner, true if a winner is found.
void gameObject::processGoal() {
    // Resets all the goals to false.
    for (int puckNumber = 0; puckNumber < maxNumberOfPucks; ++puckNumber) {
        bPuckGoal[puckNumber] = false;
    }
    goalScored = false; // Resets status of a goal
    resetTable(); // Resets table to original status.

    for (int playerN = 0; playerN < 2; ++playerN) {
       printf("playerN = %d, has a score of %d\t", playerN, playerScore[playerN]);
    } 
    std::cout << "\n";     
}

int gameObject::getPoints(const int playerN) {
    return playerScore[playerN];
}

void gameObject::setPoints(const int playerN, const int pointNumber) {
    playerScore[playerN] = pointNumber;
}

void gameObject::setLoadingScreen(bool value) {
    loadingScreen = value;
}
