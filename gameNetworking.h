//
// Created by Patrick on 2020-11-30.
//

#ifndef INC_2CARS2RACING_GAMENETWORKING_H
#define INC_2CARS2RACING_GAMENETWORKING_H
//class gameNetworking{
//public:
//    gameNetworking();
//    ~gameNetworking();
//
//
//    void gameLobby();
//    void networkGame();
//};

#include "gameObject.h"

void gameLobby();
void game();
void sendData(gameObject &game);
void receiveData(gameObject &game);

#endif //INC_2CARS2RACING_GAMENETWORKING_H
