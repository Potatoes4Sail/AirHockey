//
// Created by Patrick on 2020-11-24.
//

#include "UI_text.h"
#include "charFunctions.h"
#include "2D_graphics/2D_graphics.h"

UI_text::UI_text(const vector &_position, const char *_textValue, double scale, const char *_configValue) {
    // Sets position & scale of text object
    this->position=_position;
    this->scale = scale;

    // Stores value for textValue
    int length = lengthOfChars(_textValue) + 1;
    this->textContents = new char[length];
    for (int i = 0; i < length; ++i) {
        textContents[i] = _textValue[i];
    }

    // Stores value for configValue
    length = lengthOfChars(_configValue) + 1;
    this->configValue = new char[length];
    for (int i = 0; i < length; ++i) {
        configValue[i] = _configValue[i];
    }
}

UI_text::UI_text(const UI_text &UI_text2) {
    std::cout << "Whoa we don't do that here.\n"; // Don't want to use a copy constructor
}

UI_text::~UI_text() {
    if(textContents != nullptr){
        delete[] textContents;
        textContents = nullptr;
    }

    if(configValue != nullptr){
        delete[] configValue;
        configValue = nullptr;
    }
}


void UI_text::draw() {
    text(textContents, position.x, position.y, scale);
}

const vector &UI_text::getPosition() const {
    return position;
}

double UI_text::getScale() const {
    return scale;
}

void UI_text::updateText(const char *_textValue) {
    int length = lengthOfChars(_textValue) + 1; // Gets length of new text value

    delete [] textContents; // Deletes old text value

    this->textContents = new char[length]; // Creates new value and store this instead.
    for (int i = 0; i < length; ++i) {
        textContents[i] = _textValue[i];
    }

}

char *UI_text::getTextContents() const {
    return textContents;
}

char *UI_text::getConfigValue() const {
    return configValue;
}
