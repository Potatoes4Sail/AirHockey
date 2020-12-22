//
// Created by Patrick on 2020-11-24.
//

#ifndef INC_2CARS2RACING_MENU_H
#define INC_2CARS2RACING_MENU_H

#include "config.h"
#include "UI.h"

int mainMenu();
void menuMovement(UI &menu, const int *GS, bool &selection);

void networkSettingsMenu();
void gameSettingsMenu();

void updateTextField(char *value, bool &finishedEditing);
void modifyValue(UI &UIObj, const int textID);
void saveValues(UI &UIObj);

void scoreboardMenu();

#endif //INC_2CARS2RACING_MENU_H
