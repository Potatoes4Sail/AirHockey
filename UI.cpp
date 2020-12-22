//
// Created by Patrick on 2020-11-23.
//

#include "UI.h"
#include "charFunctions.h"

UI::UI() {
    // Sets initial number of buttons & text to 0.
    selectedButton = 0;
    numButtons = 0;
    numText = 0;

    // Sets empty pointers to nullptr to avoid trying to delete these. Can cause bad errors
    for (int currentButton = 0; currentButton < maxNumberOfButtons; ++currentButton) {
        buttonArray[currentButton] = nullptr;
    }

    for (int currentText = 0; currentText < maxNumberOfText; ++currentText) {
        textArray[currentText] = nullptr;
    }

}

UI::~UI() {
    for (int currentButton = 0; currentButton < maxNumberOfButtons; ++currentButton) {
        if (buttonArray[currentButton] != nullptr) {
            delete buttonArray[currentButton];
            buttonArray[currentButton] = nullptr;
        }
    }

    for (int currentText = 0; currentText < maxNumberOfText; ++currentText) {
        if (textArray[currentText] != nullptr) {
            delete textArray[currentText];
            textArray[currentText] = nullptr;
        }
    }
}

// Adds a new button
void UI::addButton(const vector &position, const char *buttonName) {
    if(numButtons < maxNumberOfButtons) {
        buttonArray[numButtons] = new button(position, buttonName);
        numButtons++;
    } else {
        std::cout << "WARNING: Cannot make additional button objects. Reached limit. \n";
    }

}

///  Adds text and returns ID of text item.
int UI::addText(const vector &position, const char *textValue, double scale, const char *configValue) {
    int textID = -1; // tracks return value
    if(numText < maxNumberOfText) {
        textID = numText;
        textArray[numText] = new UI_text(position, textValue, scale, configValue);
        numText++;
    } else {
        std::cout << "WARNING: Cannot make additional text objects. Reached limit. \n";
    }
    return textID;
}

// Removes everything in this class
void UI::clear() {
    clearButtons();
    clearText();
}

// Removes all buttons from class
void UI::clearButtons() {
    for (int currentButton = 0; currentButton < numButtons; ++currentButton) {
        if (buttonArray[currentButton] != nullptr) {
            delete buttonArray[currentButton];
            buttonArray[currentButton] = nullptr;
        }
    }
    numButtons = 0;
}

// Removes all text from class
void UI::clearText() {
    for (int i = 0; i < numText; ++i) {
        if(textArray[i] != nullptr) {
            delete textArray[i];
            textArray[i] = nullptr;
        }
    }
    numText = 0;
}


void UI::removeLastText() {
    if(numText > 0){ // Can't remove text if no text exists.
        if(textArray[numText] != nullptr) { // Delete that piece of text.
            delete textArray[numText];
            textArray[numText] = nullptr;
        }
        numText--; // Decrease number of text that exist
    }
}


void UI::updateLastText(const char *textValue) {
    // Updates last text item to be added.
    textArray[numText - 1]->updateText(textValue);
}

// Updates the text stored in value
void UI::updateText(const char *textValue, const int textID) {
    textArray[textID]->updateText(textValue);
}

// Returns text from button.
char *UI::getText(const int textID) {
    return textArray[textID]->getTextContents();
}

void UI::draw() {
    // Draws all the buttons
    for (int currentButton = 0; currentButton < numButtons; ++currentButton) {
        buttonArray[currentButton]->draw((selectedButton == currentButton));
    }

    for (int currentText = 0; currentText < numText; ++currentText) {
        textArray[currentText]->draw();
    }
}

void UI::moveUp() {
    if(selectedButton > 0) selectedButton--;
}

void UI::moveDown() {
    if(selectedButton < (numButtons - 1)) selectedButton++;
}

char * UI::getButtonName() const{
    if(numButtons > 0){
        return (buttonArray[selectedButton]->getName());
    }
    return nullptr;
}

// Returns value stored in UI_text object
char *UI::getTextConfig(const int textID) {
        return textArray[textID]->getConfigValue();
}

int UI::getNumberOfTextObjects() {
    return numText;
}


