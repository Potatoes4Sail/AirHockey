//
// Created by Patrick on 2020-11-24.
//

#ifndef INC_2CARS2RACING_UI_TEXT_H
#define INC_2CARS2RACING_UI_TEXT_H


#include "vector.h"

class UI_text {
    vector position;
    char *textContents;
    char *configValue;
    double scale;
public:
//    UI_text(const vector &_position, const char *_textValue, double scale = 1); // Default constructor
    UI_text(const vector &_position, const char *_textValue, double scale = 1, const char *_configValue = ""); // Default constructor
    UI_text(const UI_text &UI_text2);
    ~UI_text();
    void draw();
//    vector getPosition();
//    double getSc
    const vector &getPosition() const;
    double getScale() const;

    void updateText(const char *_textValue);

    char *getTextContents() const;
    char *getConfigValue() const;
};


#endif //INC_2CARS2RACING_UI_TEXT_H
