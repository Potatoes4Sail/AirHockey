//
// Created by Patrick on 2020-11-23.
//

#include "button.h"
#include "charFunctions.h"
#include "shapeDrawing.h"
#include "2D_graphics/2D_graphics.h"

button::button(const vector &position, const char *buttonName) {
    // Sets location for definition of buttons.
    this->position=position;
    int length = lengthOfChars(buttonName) + 1;
    name = new char[length];
    for (int i = 0; i < length; ++i) {
        name[i] = buttonName[i];
    }

    // Sets default size
    // TODO: Add ability to change size of this
    buttonWidth = 350;
    buttonHeight = 70;

    // Sets default colour of button
    colorArray[0] = 104;
    colorArray[1] = 229;
    colorArray[2] = 84;

    // Sets default colour of selected button
    selectedButtonColorArray[0] = 62;
    selectedButtonColorArray[1] = 0;
    selectedButtonColorArray[2] = 77;
}


button::button(const button &b1) {
    std::cout << "Whoa we don't do that here.\n";
}


button::~button() {
    if(name != nullptr){
        delete[] name;
        name = nullptr;
    }
}

void button::draw(bool selected) {
    double xPos[2], yPos[2];
    xPos[0] = position.x - buttonWidth / 2;
    xPos[1] = position.x + buttonWidth / 2;

    yPos[0] = position.y - buttonHeight / 2;
    yPos[1] = position.y + buttonHeight / 2;

    if(selected) {
        debugDrawRectangle((double *) selectedButtonColorArray, xPos, yPos);
    } else {
        debugDrawRectangle((double *) colorArray, xPos, yPos);
    }

    double verticalOffset = buttonHeight / 2;
    verticalOffset = 25;

    double horizontalOffset = buttonWidth * (0.36);

    text(name, position.x - horizontalOffset, position.y + verticalOffset, 1);
}

char *button::getName() const {
    return name;
}
