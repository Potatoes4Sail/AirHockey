//
// Created by Patrick on 2020-11-23.
//

#ifndef INC_2CARS2RACING_BUTTON_H
#define INC_2CARS2RACING_BUTTON_H

#include "vector.h"

class button {
    double buttonWidth, buttonHeight;
    char *name;
    vector position;
    double colorArray[3];
    double selectedButtonColorArray[3];
public:
    button(const vector &position, const char *buttonName);
    button(const button &b1);
    ~button();
    void draw(bool selected = false);

    char *getName() const;
};

#endif //INC_2CARS2RACING_BUTTON_H