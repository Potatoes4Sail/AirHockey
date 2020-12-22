//TODO: Go over file and verify functions are working properly.

#include "shapeDrawing.h"
#include <cmath>
#include "2D_graphics/2D_graphics.h"
const double PI = 2*asin(1);

void debugDrawRectangle(const double *colorsArrayFull, const double *xValues, const double *yValues)  {
    double colorArray[3][3]; ///< Colors stored in [0] = R, [1] = G, [2] = B;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            colorArray[j][i] = colorsArrayFull[j] / 256.0;
        }
    }

    double xCoordArray[3], yCoordArray[3];

    xCoordArray[0] = xValues[0];
    xCoordArray[1] = xValues[0];
    xCoordArray[2] = xValues[1];

    yCoordArray[0] = yValues[0];
    yCoordArray[1] = yValues[1];
    yCoordArray[2] = yValues[1];

    triangle(xCoordArray, yCoordArray, colorArray[0], colorArray[1], colorArray[2]);

    xCoordArray[0] = xValues[0];
    xCoordArray[1] = xValues[1];
    xCoordArray[2] = xValues[1];

    yCoordArray[0] = yValues[0];
    yCoordArray[1] = yValues[0];
    yCoordArray[2] = yValues[1];

    triangle(xCoordArray, yCoordArray, colorArray[0], colorArray[1], colorArray[2]);
}

void debugDrawQuad(const double *colorsArrayFull, const double xValues[4], const double yValues[4]) {
    double colorArray[3][3]; ///< Colors stored in [0] = R, [1] = G, [2] = B;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            colorArray[j][i] = colorsArrayFull[j] / 256.0;
        }
    }

    double xCoordsForTriangle[3], yCoordsForTriangle[3];
    xCoordsForTriangle[0] = xValues[0];
    yCoordsForTriangle[0] = yValues[0];
    for (int setOfPoints = 1; setOfPoints <= 2; ++setOfPoints) {
        for (int i = 0; i < 2; ++i) {
            xCoordsForTriangle[i + 1] = xValues[i + setOfPoints];
            yCoordsForTriangle[i + 1] = yValues[i + setOfPoints];
        }
        triangle(xCoordsForTriangle, yCoordsForTriangle, colorArray[0], colorArray[1], colorArray[2]);
    }
}

void debugDrawCircle(const double *colorsArrayFull, double xPos, double yPos, double radius) {
    double colorArray[3][3]; ///< Colors stored in [0] = R, [1] = G, [2] = B;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            colorArray[j][i] = colorsArrayFull[j] / 256.0;
        }
    }

    static int triangleCount = 30;
    double xCoords[3], yCoords[3];

    xCoords[0] = xPos;
    yCoords[0] = yPos;
    for (int i = 1; i <= triangleCount; ++i) {
        xCoords[1] = xPos + radius * cos(2 * PI / (triangleCount) * (i - 1));
        xCoords[2] = xPos + radius * cos(2 * PI / (triangleCount) * i);

        yCoords[1] = yPos + radius * sin(2 * PI / (triangleCount) * (i - 1));
        yCoords[2] = yPos + radius * sin(2 * PI / (triangleCount) * i);

        triangle(xCoords, yCoords, colorArray[0], colorArray[1], colorArray[2]);
    }
}

void debugDrawVector(const vector &centerOfVector, const vector &displacement, double scale) {
    double scalarLineX[2], scalarLineY[2];
    scalarLineX[0] = centerOfVector.x;
    scalarLineX[1] = (centerOfVector + displacement * scale).x;

    scalarLineY[0] = centerOfVector.y;
    scalarLineY[1] = (centerOfVector + displacement * scale).y;
    line(scalarLineX, scalarLineY, 2, 255, 0, 0);
}

void debugDrawVector(const vector &centerOfVector, const vector &displacement, double scale, const double *colorsArrayFull) {
    double scalarLineX[2], scalarLineY[2];
    scalarLineX[0] = centerOfVector.x;
    scalarLineX[1] = (centerOfVector + displacement * scale).x;

    scalarLineY[0] = centerOfVector.y;
    scalarLineY[1] = (centerOfVector + displacement * scale).y;
    line(scalarLineX, scalarLineY, 2, colorsArrayFull[0]/255.0, colorsArrayFull[1]/255.0, colorsArrayFull[2]/255.0);
}
