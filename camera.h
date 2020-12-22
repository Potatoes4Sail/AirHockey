//
// Created by Patrick on 2020-11-16.
//

#ifndef IN2CARS2RACING_CAMERA_H
#define IN2CARS2RACING_CAMERA_H

#include "vector.h"

static double G_CAMERA_SCALE;
static double G_CAMERA_ANGLE;
static vector G_CAMERA_POSITION;
static vector G_CAMERA_CENTER_OF_SCREEN;

/// Initializes camera to know where the center of the screen should be (coordinates)
void initCamera(const vector &_centerOfScreen);

/// Functions to position camera where it should be located.
/// @param _position    position of center of camera, in a vector of form (x,y)
/// @param _scale       scale of camera, each pixel is scaled by this value
/// @param _angle       angle of rotation of camera in radians.
/// @return if return is false, encountered an error.
bool setCamera(const vector &_position, const double &_scale, const double &_angle = 0);

// Functions to draw stuff (using 2D_graphics library).
void drawLine(const double x[], const double y[], const int n, const double R, const double G, const double B);
void drawTriangle(const double x[], const double y[], double R[], double G[], double B[]);
void drawText(const char *str, const double xp, const double yp, double _scale = 1.0);
void drawText(double value, const double xp, const double yp, double _scale = 1.0);
void drawSprite(int id, const double x, const double y, double theta, double _scale);

// Functions to draw stuff (using shapeDrawing.cpp)
void drawRectangle(const double *colorsArrayFull, const double xValues[2], const double yValues[2]);
void drawQuad(const double *colorsArrayFull, const double xValues[4], const double yValues[4]);
void drawCircle(const double *colorsArrayFull, double xPos, double yPos, double radius);
void drawCircle(const double *colorsArrayFull, vector &pos, double radius);

void drawRotatedRectangle(const double *colorsArrayFull, const vector &_position, const double _width, const double _height, const double _angle);
void drawVector(const vector &centerOfVector, const vector &displacement, double scale);
void drawVector(const vector &centerOfVector, const vector &displacement, double scale, const double *colorsArrayFull);

// Functions only used in this part.
double scaleX(const double x);
double scaleY(const double y);

#endif //IN2CARS2RACING_CAMERA_H
