#include "camera.h"
#include "shapeDrawing.h"
#include "2D_graphics/2D_graphics.h"
#include <iostream>

void initCamera(const vector &_centerOfScreen){
    G_CAMERA_CENTER_OF_SCREEN = _centerOfScreen;
}

bool setCamera(const vector &_position, const double &_scale, const double &_angle) {
    // Verifies the position for camera is valid.
    if(_position.x < 0 || _position.y < 0) { // This would be to the left/below, probably want to avoid but it's not strictly incorrect for this to happen.
//        return false;
    }
    G_CAMERA_POSITION = _position;

    // Ensures scale is valid (ie, > 0)
    if(_scale > 0) {
        if(_scale < 1) {
            // This should only be a warning, it's possible for this to be desired but I highly doubt it.
        }
        G_CAMERA_SCALE = _scale;
    }

    // Any value for the angle is valid.
    G_CAMERA_ANGLE = _angle;

    return true;
}

void drawLine(const double *x, const double *y, const int n, const double R, const double G, const double B) {
    auto *scaledX = new double[n];
    auto *scaledY = new double[n];
    // scales line to camera.

    for (int point = 0; point < n; ++point) {
        scaledX[point] = scaleX(x[point]);
        scaledY[point] = scaleY(y[point]);
    }

    line(scaledX, scaledY, n, R, G, B);

    delete [] scaledX;
    delete [] scaledY;
}

double scaleX(const double x) {
    // Debugging statements
//    double newX = (x - G_CAMERA_POSITION.x) * G_CAMERA_SCALE + G_CAMERA_CENTER_OF_SCREEN.x;
//    printf ("x\tInitial value = %f, is now %f\t", x, newX);
    return (x - G_CAMERA_POSITION.x) * G_CAMERA_SCALE + G_CAMERA_CENTER_OF_SCREEN.x;
}

double scaleY(const double y) {
    // Debugging statements
//    double newY = (y - G_CAMERA_POSITION.y) * G_CAMERA_SCALE + G_CAMERA_CENTER_OF_SCREEN.y;
//    printf ("y\tInitial value = %f, is now %f\n", y, newY);
    return (y - G_CAMERA_POSITION.y) * G_CAMERA_SCALE + G_CAMERA_CENTER_OF_SCREEN.y;
}


void drawTriangle(const double *x, const double *y, double *R, double *G, double *B) {
    double scaledX[3];
    double scaledY[3];

    // scales triangle relative to camera.
    for (int point = 0; point < 3; ++point) {
        scaledX[point] = scaleX(x[point]);
        scaledY[point] = scaleY(y[point]);
    }

    triangle(scaledX, scaledY, R, G, B);
}

void drawText(const char *str, const double xp, const double yp, double _scale) {
    double scaledX;
    double scaledY;
    scaledX = scaleX(xp);
    scaledY = scaleY(yp);

    text(*str, scaledX, scaledY, _scale);
}

void drawText(double value, const double xp, const double yp, double _scale) {
    double scaledX;
    double scaledY;
    scaledX = scaleX(xp);
    scaledY = scaleY(yp);

    text(value, scaledX, scaledY, _scale);
}

void drawSprite(int id, const double x, const double y, double theta, double _scale) {
    double scaledX;
    double scaledY;
    scaledX = scaleX(x);
    scaledY = scaleY(y);

    draw_sprite(id, scaledX, scaledY, theta, _scale);
}

void drawRectangle(const double *colorsArrayFull, const double xValues[2], const double yValues[2]) {
    double scaledX[2];
    double scaledY[2];

    // scales triangle relative to camera.
    for (int point = 0; point < 2; ++point) {
        scaledX[point] = scaleX(xValues[point]);
        scaledY[point] = scaleY(yValues[point]);
    }

    debugDrawRectangle(colorsArrayFull, scaledX, scaledY);
}

void drawQuad(const double *colorsArrayFull, const double xValues[4], const double yValues[4]) {
    double scaledX[4];
    double scaledY[4];

    // scales triangle relative to camera.
    for (int point = 0; point < 4; ++point) {
        scaledX[point] = scaleX(xValues[point]);
        scaledY[point] = scaleY(yValues[point]);
    }
    debugDrawQuad(colorsArrayFull, scaledX, scaledY);
}

void drawCircle(const double *colorsArrayFull, double xPos, double yPos, double radius) {
    double scaledX;
    double scaledY;
    double scaledRadius;

    scaledX = scaleX(xPos);
    scaledY = scaleY(yPos);
    scaledRadius = radius * G_CAMERA_SCALE;

    debugDrawCircle(colorsArrayFull, scaledX, scaledY, scaledRadius);
}

void drawCircle(const double *colorsArrayFull, vector &pos, double radius) {
    drawCircle(colorsArrayFull, pos.x, pos.y, radius);
}

void drawRotatedRectangle(const double *colorsArrayFull, const vector &_position, const double _width, const double _height, const double _angle) {
    vector corners[4]; ///< Vector array for each of the four corners.
    corners[0] = vector(-_width / 2.0, -_height / 2.0);
    corners[1] = vector(-_width / 2.0, +_height / 2.0);
    corners[2] = vector(+_width / 2.0, +_height / 2.0);
    corners[3] = vector(+_width / 2.0, -_height / 2.0);

    // Create a rotation matrix
    Mat22 rotationMatrix(_angle);
    // Rotate vectors by rotation matrix.
    for (auto &corner : corners) {
        corner = rotationMatrix * corner;
    }

    // Create array to hold final x & y positions
    double xPositions[4];
    double yPositions[4];

    // Set initial array equal to the corners.
    for (int i = 0; i < 4; ++i) {
        xPositions[i] = _position.x + corners[i].x;
        yPositions[i] = _position.y + corners[i].y;
    }

    drawQuad(colorsArrayFull, xPositions, yPositions);
}

void drawVector(const vector &centerOfVector, const vector &displacement, double scale) {
    // Scales x & y (tip) of vector and draws vector function.
    double scaledX = scaleX(centerOfVector.x);
    double scaledY = scaleY(centerOfVector.y);
    vector scaledCenterOfVector(scaledX, scaledY);
    debugDrawVector(scaledCenterOfVector, displacement, scale);
}

void drawVector(const vector &centerOfVector, const vector &displacement, double scale, const double colorsArrayFull[3]) {
    double scaledX = scaleX(centerOfVector.x);
    double scaledY = scaleY(centerOfVector.y);
    vector scaledCenterOfVector(scaledX, scaledY);
    debugDrawVector(scaledCenterOfVector, displacement, scale, colorsArrayFull);
}
