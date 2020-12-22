//
// Created by Patrick on 2020-11-26.
//

#ifndef INC_2CARS2RACING_SHAPES_H
#define INC_2CARS2RACING_SHAPES_H

#include "vector.h"

class shapes {

};

class sLine : protected shapes {
    vector end1, end2;
public:
    sLine(const vector &end1, const vector &end2);
    sLine(double x1, double y1, double x2, double y2);

    void draw();
    double distanceToPoint(const vector &p);
    vector vectorToPoint(const vector &p);
    vector getLineVector() const;
};

class arc : protected shapes {
    vector center;
    double radius;
    double theta0;
    double theta1;
public:
    arc(double centerX, double centerY, double radius, double theta0, double theta1);

    void draw() const;
    vector vectorToPoint(const vector &p);
};
#endif //INC_2CARS2RACING_SHAPES_H
