#include "gameNetworking.h"
#include "2D_graphics/UDP_com.h"
#include "charFunctions.h"
#include "2D_graphics/2D_graphics.h"
#include "config.h"
#include "camera.h"
#include "binaryIO.h"
#include "gameInfo.h"
#include "ran.h"
#include "2D_graphics/UDP_com6.h"
#include <Windows.h>
#include "2D_graphics/timer.h"
#include "customMath.h"             
#include "2D_graphics/game_pad.h"

// This value should be changed between builds to ensure you have two clients
// It is currently done by having two different builds. "Debug and DebugHost"
#ifdef _HOST_BUILD
#define HOST true
#else
#define HOST false
#endif

extern config *configFile;
static const int NETWORK_BUFFER_SIZE = 10000;

void clearBuffer(char *buffer);

void recordScore(gameObject &game);

void AI_move(const gameObject &game, int AILevel, int paddleNumber = 1);

void gameLobby(int playerNumber);

gameInfo gInfo;

long int seed = -42; // Seed for AI movement.

//Create buffers
char sendBuffer[NETWORK_BUFFER_SIZE];
char receiveBuffer[NETWORK_BUFFER_SIZE];

int sock; // UDP socket ID number
int port = 37000; // socket port number
int size; // size of sent/received message (bytes/characters)

// local IP address associated with this socket
char IP_address_local[NMAX_ADDRESS];

// IP address of message received
char IP_address_recv[NMAX_ADDRESS];

// IP address where the initial message is sent (ie the server)
char IP_address_server[NMAX_ADDRESS];

// Following functions used to generalize networking with either IPv4 or IPv6.
bool IPv4 = true; // Stores if game is using IPv6. By default this should be true, however can be set to false in config.

int genericActivateSocket(int _port, char *local_IP_address, int &_sock) {
    if (IPv4 == 1) { // Use IPv4
        return activate_socket(_port, local_IP_address, _sock);
    } else { // Use IPv6
        return activate_socket6(_port, local_IP_address, _sock);
    }
}

int genericDeactivateSocket6(int &_sock) {
    if (IPv4 == 1) { // Use IPv4
        return deactivate_socket(_sock);
    } else { // Use IPv6
        return deactivate_socket6(_sock);
    }
}

int genericRecv(char *buffer, int &_size, char *IP_address, int _sock) {
    if (IPv4 == 1) { // Use IPv4
        return recv(buffer, _size, IP_address, _sock);
    } else { // Use IPv6
        return recv6(buffer, _size, IP_address, _sock);
    }
}

int genericSend(char *buffer, int _size, char *IP_address, int _sock, int _port) {
    if (IPv4 == 1) { // Use IPv4
        return send(buffer, _size, IP_address, _sock, _port);
    } else { // Use IPv6
        return send6(buffer, _size, IP_address, _sock, _port);
    }
}

void game() { //bool networkGame, int numberOfPlayers) {
    // Setup for game object.

    // Loads needed values from config.

    IPv4 = configFile->getBoolValue("IPv4");
    bool networkGame = configFile->getBoolValue("NETWORK_GAME");
    int numberOfPlayers = configFile->getIntValue("NUMBER_OF_PLAYERS");
    int AILevel = configFile->getIntValue("AI_LEVEL");

    // Debug statement
    printf("networkGame = %d, numberOfPlayers = %d, AI level = %d\n", networkGame, numberOfPlayers, AILevel);

    // If network game has been selected, need to setup lobby and the such first before game can be played.

    gInfo.setMap(configFile->getCharValue("TABLE_FILE"));
    gInfo.setPlayer0Name(configFile->getCharValue("PLAYER_NAME1"));
    gInfo.setPlayer1Name(configFile->getCharValue("PLAYER_NAME2"));

    if (networkGame) {
        gameLobby(); // TODO: Improve this functionality
    }

    // Creates camera
    vector vCamera(0, 0);
    double scale = 250, cameraAngle = 0;
    setCamera(vCamera, scale, cameraAngle);

    // Creates game object.
    gameObject game;
    char tableName[24];
    strcpy(tableName, configFile->getCharValue("TABLE_FILE"));
    game.loadTable((char *) tableName);

    int playerA = 0, playerB = 1;
    // Assigns proper player numbers
    if (networkGame) {
        playerA = HOST; // Reassigns playerA to if the player is running the host client or not
    }


    bool continueLoop = true;
    int GS[N_GS]; // Holds Gamepad info. 
    int deadband = 1000000 * 0.1; // Anything below 10% movement won't register.
    do {

        if (gamepad_state(GS, 1) == 1) { // This will return 1 if issue with function
                // Want to ignore invalid gamepad state so will set everything equal 0;
            for (int i = 0; i < N_GS; ++i) {
                GS[i] = 0;
            }
        }

        clear();
        double speed = .25;

        if (networkGame) { // Only need to process network buffers if game is a network game.
            // Clears both buffers
            clearBuffer(sendBuffer);
            clearBuffer(receiveBuffer);

            // Fill send buffer with required information from game.
            sendData(game);
            receiveData(game);
        }

        // Check if a goal was scored.
        if (game.goalScored) { // A goal was scored. Process needed goal items here.
            game.processGoal();
        }

        // Checks if a player has won.
        int maxPointsPerGame = gInfo.maxScore;
        for (int playerN = 0; playerN < 2; ++playerN) {
            if (game.getPoints(playerN) >= maxPointsPerGame) {
                printf("playerN = %d won! with a score of %d\n", playerN, game.getPoints(playerN));
                continueLoop = false; // A player has won, exit game.
            }
        }

        double deltaVX = 0, deltaVY = 0;
        // Gets input from Gamepad
        if(GS[RIGHT_STICK_Y] > deadband) deltaVY = GS[RIGHT_STICK_Y] * speed / 1000000;
        if(GS[RIGHT_STICK_Y] < -deadband) deltaVY = GS[RIGHT_STICK_Y] * speed / 1000000;
        if(GS[RIGHT_STICK_X] >  deadband) deltaVX = GS[RIGHT_STICK_X] * speed / 1000000;
        if(GS[RIGHT_STICK_X] < -deadband) deltaVX = GS[RIGHT_STICK_X] * speed / 1000000;

        // Gets keyboard input
        if (KEY('W')) deltaVY = +speed;
        if (KEY('S')) deltaVY = -speed;
        if (KEY('D')) deltaVX = +speed;
        if (KEY('A')) deltaVX = -speed;

        // Sets the velocity of the player
        game.paddleArray[playerA]->object.velocity.y += deltaVY;
        game.paddleArray[playerA]->object.velocity.x += deltaVX;

        if (!networkGame) {
            if (numberOfPlayers == 2) {
                if (KEY('I')) game.paddleArray[playerB]->object.velocity.y += speed;
                if (KEY('K')) game.paddleArray[playerB]->object.velocity.y -= speed;
                if (KEY('L')) game.paddleArray[playerB]->object.velocity.x += speed;
                if (KEY('J')) game.paddleArray[playerB]->object.velocity.x -= speed;
            } else {
                AI_move(game, AILevel, 1);
                //AI_move(game, 1, 0);
            }
        }

        if (KEY('R')) game.resetTable();

        game.update();
        game.draw();


        update();
    } while (!KEY('Q') && continueLoop);

    // If networking was used, need to disable at end of game.
    if (networkGame) {
        Sleep(500);
        // Turn off networking
        genericDeactivateSocket6(sock); // Use 'generic' function for IPv4 or IPv6
        deactivate_network();
    }

    recordScore(game);
}

void AI_move(const gameObject &game, int AILevel, int paddleNumber) {// TODO: Add AI movement here!
    double speedFactor = .15;
    if (AILevel == 0) { // This will just simply move towards the puck, it's very bad.
        game.paddleArray[paddleNumber]->setVelocity(game.paddleArray[paddleNumber]->getVelocity() +
                                                    unit(game.puckArray[0]->getPosition() -
                                                         game.paddleArray[paddleNumber]->getPosition()) * speedFactor);

    } else if (AILevel == 1) {
        int closestPuckNumber = 0;
        double distanceToPuck = distance(game.puckArray[0]->getPosition(),
                                         game.pTable->getPaddleStartingPos(paddleNumber));
        for (int i = 0; i < game.numberOfPucks; ++i) {
            if (distanceToPuck >=
                distance(game.puckArray[i]->getPosition(), game.pTable->getPaddleStartingPos(paddleNumber))) {
                // Found a closer puck.
                closestPuckNumber = i;
//                distanceToPuck = distance(game.puckArray[i]->getPosition(), game.pTable->getPaddleStartingPos(paddleNumber));
            }
            // This puck is on the side.
        }

        game.paddleArray[paddleNumber]->setVelocity(game.paddleArray[paddleNumber]->getVelocity() +
                                                    unit(game.puckArray[closestPuckNumber]->getPosition() -
                                                         game.paddleArray[paddleNumber]->getPosition()) * speedFactor);
    } else if (AILevel == 2) { // Checks if there is a puck on it's side, other wise move back to start.

        int closestPuckNumber = -1;
        double distanceToPuck = distance(game.puckArray[0]->getPosition(),
                                         game.pTable->getPaddleStartingPos(paddleNumber));
        bool puckOnSide = false;
        for (int i = 0; i < game.numberOfPucks; ++i) {
            if (game.puckArray[i]->getPosition().x * game.pTable->getPaddleStartingPos(paddleNumber).x >= 0) {
                puckOnSide = true;
                if (distanceToPuck >=
                    distance(game.puckArray[i]->getPosition(), game.pTable->getPaddleStartingPos(paddleNumber))) {
                    // Found a closer puck.
                    closestPuckNumber = i;
                    distanceToPuck = distance(game.puckArray[i]->getPosition(),
                                              game.pTable->getPaddleStartingPos(paddleNumber));
                }
                // This puck is on the side.
            }
        }

        if (puckOnSide) { // Move towards puck.
            game.paddleArray[paddleNumber]->setVelocity(game.paddleArray[paddleNumber]->getVelocity() +
                                                        unit(game.puckArray[closestPuckNumber]->getPosition() -
                                                             game.paddleArray[paddleNumber]->getPosition()) *
                                                        speedFactor);
        } else { // Move back to original spot.
            game.paddleArray[paddleNumber]->setVelocity(game.paddleArray[paddleNumber]->getVelocity() +
                                                        unit(game.pTable->getPaddleStartingPos(paddleNumber) -
                                                             game.paddleArray[paddleNumber]->getPosition()) *
                                                        speedFactor);
        }

        // Gets the closest puck
    }

    // Limit the max speed of the AI
    double maxSpeed = 10;
    if (norm(game.paddleArray[paddleNumber]->getVelocity()) > maxSpeed) {
        game.paddleArray[paddleNumber]->setVelocity(unit(game.paddleArray[paddleNumber]->getVelocity()) * maxSpeed);
    }

    // Randomize motion slightly
    paddle *p = game.paddleArray[paddleNumber];
    vector velocity = p->getVelocity();
    velocity.x = velocity.x * (ran(seed) * 0.1 + .95) + (ran(seed) - 0.5) * 0.05;
    velocity.y = velocity.y * (ran(seed) * 0.1 + .95) + (ran(seed) - 0.5) * 0.05;
    p->setVelocity(velocity);
}

void recordScore(gameObject &game) {// Stores name & score in binary file.
    binaryIO scoreFile("scoreboard.bin");

    // Size of two character names + score for player 1 & player 2
    int lengthOfNewBuffer = 16 * sizeof(char) * 2 + sizeof(int) * 2 * 2;
    char *newBuffer = new char[lengthOfNewBuffer];

    // Sets everything in the buffer to 0 to avoid having issues with random junk in memory
    for (int i = 0; i < lengthOfNewBuffer; ++i) {
        newBuffer[i] = 0;
    }

    int sizeOfBuffer = 0; // Variable which is used to store length of buffer
    char *pChar; // Holds a char pointer


    // Stores first name
    for (int i = 0; i < 16; ++i) { // Loops through each character of name.
        pChar = (char *) (newBuffer + sizeOfBuffer);
        if (i < lengthOfChars(gInfo.player0Name)) { // Only sets values if they are in the name
            *pChar = gInfo.player0Name[i];
        }
        sizeOfBuffer += sizeof(*pChar);
    }

    // Stores second name
    for (int i = 0; i < 16; ++i) { // Loops through each character of name.
        pChar = (char *) (newBuffer + sizeOfBuffer);
        if (i < lengthOfChars(gInfo.player1Name)) { // Only sets values if they are in the name
            *pChar = gInfo.player1Name[i];
        }
        sizeOfBuffer += sizeof(*pChar);
    }

    int *pInt; // Holds an int pointer

    // Stores score in buffer.
    for (int playerN = 0; playerN < 2; ++playerN) {
        pInt = (int *) (newBuffer + sizeOfBuffer);
        *pInt = game.getPoints(playerN);
        sizeOfBuffer += sizeof(*pInt);
    }

    sizeOfBuffer += 8; // Move another 8 values down the line.

    scoreFile.addData(newBuffer, sizeOfBuffer);
}

int gameLobbyFillBuffer() {
    int sizeOfBuffer = 0;

    // Holds a int pointer;
    int *pInt;

    // Stores max score.
    pInt = (int *) (sendBuffer + sizeOfBuffer);
    *pInt = gInfo.maxScore;
    sizeOfBuffer += sizeof(*pInt);

    char *pChar; // Holds a char pointer

    // Stores map.
    for (int i = 0; i < 16; ++i) {
        pChar = (char *) (sendBuffer + sizeOfBuffer);
        *pChar = gInfo.map[i];
        sizeOfBuffer += sizeof(*pChar);
    }

    // Stores player 0 name
    for (int i = 0; i < 16; ++i) {
        pChar = (char *) (sendBuffer + sizeOfBuffer);
        *pChar = gInfo.player0Name[i];
        sizeOfBuffer += sizeof(*pChar);
    }

    // Stores player 1 name
    for (int i = 0; i < 16; ++i) {
        pChar = (char *) (sendBuffer + sizeOfBuffer);
        *pChar = gInfo.player1Name[i];
        sizeOfBuffer += sizeof(*pChar);
    }
    return sizeOfBuffer;
}

void gameLobbyRecieve() {
    int sizeOfBuffer = 0;
    char *pChar; // Character pointer
    int *pInt; // int pointer
    if (HOST) {
        // Is host, only get other player's name
        sizeOfBuffer = 20;
        for (int i = 0; i < 16; ++i) {
            pChar = (char *) (receiveBuffer + sizeOfBuffer);
            gInfo.player0Name[i] = *pChar;
            sizeOfBuffer += sizeof(*pChar);
        }
    } else {
        // Not host need all other values.
        // Get max score.
        pInt = (int *) (sendBuffer + sizeOfBuffer);
        gInfo.maxScore = *pInt;
        sizeOfBuffer += sizeof(*pInt);

        // Stores map.
        for (int i = 0; i < 16; ++i) {
            pChar = (char *) (sendBuffer + sizeOfBuffer);
            gInfo.map[i] = *pChar;
            sizeOfBuffer += sizeof(*pChar);
        }

        sizeOfBuffer += 16; // Skips setting current name.

        // Stores player 1 name
        for (int i = 0; i < 16; ++i) {
            pChar = (char *) (sendBuffer + sizeOfBuffer);
            gInfo.player1Name[i] = *pChar;
            sizeOfBuffer += sizeof(*pChar);
        }
    }
}

void gameLobby() {
    // Sets two char arrays for the HOST_IP & CLIENT_IP address (will be set to different values shortly)
    char HOST_IP[NMAX_ADDRESS];  // IP of desktop (running AirHockeyHost.exe)
    char CLIENT_IP[NMAX_ADDRESS]; // IP of laptop  (running AirHockey.exe)

    // sets value of host IP from config file.
    for (int i = 0; i < lengthOfChars(configFile->getCharValue("HOST_IP")) + 1; ++i) {
        HOST_IP[i] = configFile->getCharValue("HOST_IP")[i];
    }

    // sets value of client IP from config file
    for (int i = 0; i < lengthOfChars(configFile->getCharValue("CLIENT_IP")) + 1; ++i) {
        CLIENT_IP[i] = configFile->getCharValue("CLIENT_IP")[i];
    }

    // Clears buffers
    clearBuffer(sendBuffer);
    clearBuffer(receiveBuffer);

    if (HOST) {
        for (int i = 0; i < NMAX_ADDRESS; ++i) {
            IP_address_local[i] = HOST_IP[i];
            IP_address_server[i] = CLIENT_IP[i];
        }
    } else {
        for (int i = 0; i < NMAX_ADDRESS; ++i) {
            IP_address_local[i] = CLIENT_IP[i];
            IP_address_server[i] = HOST_IP[i];
        }
    }

    // Turn on networking
    activate_network();
    genericActivateSocket(port, IP_address_local, sock); // Use 'generic' function for IPv4 or IPv6

    // Creates menu to display info about networking status.

    double CENTER_X = configFile->getDoubleValue("WINDOW_WIDTH") / 2;
    double TOP_BUTTON = 0.6875 * configFile->getDoubleValue("WINDOW_HEIGHT");
    double SPACING = 125;

    // Creates text on top of menu
    vector t1(CENTER_X - 250, TOP_BUTTON + SPACING * 1.5);


    UI networkLobby;
    networkLobby.addText(t1, "Lobby", 2); // Draw text at top for lobby
    networkLobby.addText(vector(100, 500), (char *) "Connecting, please wait."); // Add info text.
    clear();

    networkLobby.draw(); // Draws text & update
    update();

    bool continueLoop = true;

    int loadingID = 0;
    create_sprite((char *) "assets/images/loading.png", loadingID);
    do {
        // Clears both buffers to avoid strange things occurring.
        clear();
        clearBuffer(sendBuffer);
        clearBuffer(receiveBuffer);

        int sizeOfSendBuffer = gameLobbyFillBuffer(); // Send a character to establish a connection.

        genericSend(sendBuffer, sizeOfSendBuffer, IP_address_server, sock, port); // Sends a message to the client

        if (genericRecv(receiveBuffer, size, IP_address_recv, sock) > 0) {
            // A message was received, can begin to setup game
            std::cout << "Got a response!\n";

            gameLobbyRecieve(); // Stores values from recieved into object used later on.
            networkLobby.updateLastText((char *) "Got a response! Beginning setup.");

            networkLobby.draw();
            update();
            Sleep(250);
            clear();

            for (int i = 0; i < 5; ++i) {
                genericSend(sendBuffer, sizeOfSendBuffer, IP_address_server, sock, port);
                // Sends a message to the client
                Sleep(200);
            }

            continueLoop = false; // exits loop & can now launch game

        } else {
            networkLobby.updateLastText((char *) "Waiting on a response!");
        }

        draw_sprite(loadingID, CENTER_X + 125, TOP_BUTTON + 130, -mod(high_resolution_time() * 2, 3.141592 * 2), .25);

        // Draw graphics
        networkLobby.draw();
        update();
        Sleep(20);
        
    } while (continueLoop);

    // Clear any recieved packets to avoid issues later on.
    while (genericRecv(receiveBuffer, size, IP_address_recv, sock) > 0);

}

void clearBuffer(char *buffer) {
    for (int bufferChar = 0; bufferChar < NETWORK_BUFFER_SIZE; ++bufferChar) {
        buffer[bufferChar] = 0;
    }
}

void sendData(gameObject &game) {
    int sizeOfBuffer = 0; // Stores location in buffer.

    // Puts number of pucks into buffer.
    int numOfPucks = game.numberOfPucks;

    vector *pVector; ///< Holds a vector pointer
    // If player is not the host then update puck.
    for (int i = 0; i < numOfPucks; ++i) {
        pVector = (vector *) (sendBuffer + sizeOfBuffer);
        *pVector = game.puckArray[i]->getPosition();
        sizeOfBuffer += sizeof(*pVector);

        pVector = (vector *) (sendBuffer + sizeOfBuffer);
        *pVector = game.puckArray[i]->getVelocity();
        sizeOfBuffer += sizeof(*pVector);
    }

    pVector = (vector *) (sendBuffer + sizeOfBuffer);
    *pVector = game.paddleArray[HOST]->getPosition();
    sizeOfBuffer += sizeof(*pVector);

    pVector = (vector *) (sendBuffer + sizeOfBuffer);
    *pVector = game.paddleArray[HOST]->getVelocity();
    sizeOfBuffer += sizeof(*pVector);

    int *pInt; ///<Holds a int pointer
    for (int i = 0; i < 2; ++i) {
        pInt = (int *) (sendBuffer + sizeOfBuffer);
        *pInt = game.getPoints(i);
        sizeOfBuffer += sizeof(*pInt);
    }

    bool *pBool; ///< Holds a bool pointer

    pBool = (bool *) (sendBuffer + sizeOfBuffer);
    *pBool = game.goalScored; // Checks if a goal was scored.
    sizeOfBuffer += sizeof(*pInt);

    pBool = (bool *) (sendBuffer + sizeOfBuffer);
    *pBool = game.bResetTable; // Checks if a reset was done
    sizeOfBuffer += sizeof(*pInt);

    // TODO: Add variables for the score and to reset playing field

    genericSend(sendBuffer, sizeOfBuffer, IP_address_server, sock, port);
}

void receiveData(gameObject &game) {
    // This will loop through packets until there's non left to avoid sync issues.
    // nom nom nom.
    while (genericRecv(receiveBuffer, size, IP_address_recv, sock) > 0) {
        int sizeOfBuffer = 0; // Stores location in buffer.

        int numberOfPucks = game.numberOfPucks;

        vector *pVector; ///< Holds a vector pointer

        for (int i = 0; i < numberOfPucks; ++i) {
            pVector = (vector *) (receiveBuffer + sizeOfBuffer);
            if (!HOST) game.puckArray[i]->setPosition(*pVector);
            sizeOfBuffer += sizeof(*pVector);

            pVector = (vector *) (receiveBuffer + sizeOfBuffer);
            if (!HOST) game.puckArray[i]->setVelocity(*pVector);
            sizeOfBuffer += sizeof(*pVector);
        }

        pVector = (vector *) (receiveBuffer + sizeOfBuffer);
        int b = HOST ? 0 : 1;

        game.paddleArray[b]->setPosition(*pVector);
        sizeOfBuffer += sizeof(*pVector);

        pVector = (vector *) (receiveBuffer + sizeOfBuffer);
        game.paddleArray[b]->setVelocity(*pVector);
        sizeOfBuffer += sizeof(*pVector);

        //! Need to be converted

        int *pInt; ///<Holds a int pointer
        for (int i = 0; i < 2; ++i) {
            pInt = (int *) (sendBuffer + sizeOfBuffer);
            game.setPoints(i, *pInt);
            sizeOfBuffer += sizeof(*pInt);
        }

        bool *pBool; ///< Holds a bool pointer

        pBool = (bool *) (sendBuffer + sizeOfBuffer);
        if (*pBool) {
            game.processGoal();  // Checks if a goal was scored.
        }
        sizeOfBuffer += sizeof(*pInt);

        pBool = (bool *) (sendBuffer + sizeOfBuffer);
        game.bResetTable = *pBool; // Checks if a reset was done
        sizeOfBuffer += sizeof(*pInt);
    }
}