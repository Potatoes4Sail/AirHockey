#ifndef INC_2CARS2RACING_SHAPEDRAWING_H
#define INC_2CARS2RACING_SHAPEDRAWING_H

#include "vector.h"

void debugDrawRectangle(const double *colorsArrayFull, const double *xValues, const double *yValues);
void debugDrawQuad(const double *colorsArrayFull, const double nxValues[4], const double nyValues[4]);
void debugDrawCircle(const double *colorsArrayFull, double xPos, double yPos, double radius);
void debugDrawVector(const vector &centerOfVector, const vector &displacement, double scale);
void debugDrawVector(const vector &centerOfVector, const vector &displacement, double scale, const double *colorsArrayFull);
#endif //INC_2CARS2RACING_SHAPEDRAWING_H
