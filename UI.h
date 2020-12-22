//
// Created by Patrick on 2020-11-23.
//

#ifndef INC_2CARS2RACING_UI_H
#define INC_2CARS2RACING_UI_H

static const int maxNumberOfButtons = 10;
static const int maxNumberOfText = 10;

#include "vector.h"
#include "button.h"
#include "UI_text.h"

// UI Class to do UI stuff
class UI {
    int selectedButton;
    int numButtons;
    button *buttonArray[maxNumberOfButtons]{};
    int numText;
    UI_text *textArray[maxNumberOfText]{};

public:
    UI();

    ~UI();

    // Adds buttons
    void addButton(const vector &position, const char *buttonName);

    int addText(const vector &position, const char *textValue, double scale = 1, const char *configValue = "");

    // Removes all existing items buttons
    void clear();

    // Removes all existing buttons
    void clearButtons();

    // Removes all existing text
    void clearText();

    // Removes last text
    void removeLastText();

    // Updates last text object.
    void updateLastText(const char *textValue);

    // Updates text object N
    void updateText(const char *textValue, const int textID);

    // Returns text value;
    char *getText(const int textID);

    // Returns text config value
    char *getTextConfig(const int textID);

    // Move in menu
    void moveUp();

    void moveDown();

    // Gets current name of button being used.
    char *getButtonName() const;

    // Draws menu returns true if successful, false if button doesn't exist. (IE. 0 buttons)
    void draw();

    // Returns number of text objects which exist
    int getNumberOfTextObjects();
};


#endif //INC_2CARS2RACING_UI_H
