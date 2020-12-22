//
// Created by Patrick on 2020-12-17.
//

#ifndef INC_2CARS2RACING_GAMEINFO_H
#define INC_2CARS2RACING_GAMEINFO_H

#include "charFunctions.h"

struct gameInfo {
    int maxScore = 10;
    char map[16]{};
    char player0Name[16]{};
    char player1Name[16]{};

    void setMap(char *map){
        storeChar(this->map, map);
    }

    void setPlayer0Name(char *name){
        storeChar(player0Name, name);
    }

    void setPlayer1Name(char *name){
        storeChar(player1Name, name);
    }

private:
    void storeChar(char *store, const char *input) {
        int lengthOfInput = lengthOfChars(input);
        for (int i = 0; i < 16; ++i) {
            if (i < lengthOfInput) {
                store[i] = input[i];
            } else {
                store[i] = 0;
            }
        }
    }
};

#endif //INC_2CARS2RACING_GAMEINFO_H
