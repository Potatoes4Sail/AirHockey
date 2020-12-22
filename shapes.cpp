//
// Created by Patrick on 2020-11-26.
//

#include "shapes.h"
#include "camera.h"
#include "customMath.h"

bool debugVectorDrawing = false;

sLine::sLine(const vector &end1, const vector &end2) {
    this->end1 = end1;
    this->end2 = end2;
}

sLine::sLine(double x1, double y1, double x2, double y2) {
    // Sets the two ends of the sLine to the given values.
    end1.x = x1;
    end1.y = y1;
    end2.x = x2;
    end2.y = y2;
}

void sLine::draw() {
    // Create temporary arrays for x and y coords.
    double x[2], y[2];
    x[0] = end1.x;
    x[1] = end2.x;

    y[0] = end1.y;
    y[1] = end2.y;

    // Draws line
    drawLine(x, y, 2, 0, 0, 0);
}

double sLine::distanceToPoint(const vector &p) {
    double x0 = p.x;
    double y0 = p.y;
    
    double x1 = end1.x;
    double y1 = end1.y;

    double x2 = end2.x;
    double y2 = end2.y;


    return abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) / sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

vector sLine::vectorToPoint(const vector &p) {
    vector lineVector = vector(end2 - end1); // Gets vector for length of line
    vector lineUnitVector = unit(lineVector); // Gets unit vector of line
    double lineLength = norm(lineVector); // Gets the length of the line

    vector pointVector = vector(p - end1); // Vector from start of line to point
    vector pointVectorScaled = pointVector * (1.0 / lineLength); // Gets a scaled length of point vector
    double t = dotProduct(lineUnitVector, pointVectorScaled); // get t (location on line)
    // Ensure t is between 0 & 1. ie, on the line
    if (t < 0.0) {
        t = 0;
    } else if (t > 1.0) {
        t = 1;
    }

    vector nearestPoint = lineVector * t;
    double distance = norm(pointVector - nearestPoint);
    nearestPoint = nearestPoint + end1;

    if (debugVectorDrawing) {
        double lineStartToPuck[3]{87, 239, 50}; // green
        drawVector(end1, pointVector, 250, lineStartToPuck);

        double lengthAlongLine[3]{250, 132, 7}; //orange
        drawVector(end1, lineVector * t, 250, lengthAlongLine);

        double pointToPuckColor[3]{239, 50, 81}; // red
        drawVector(nearestPoint, pointVector - lineVector * t, 250, pointToPuckColor);
    }


    return (pointVector - lineVector * t);
}

vector sLine::getLineVector() const {
    return vector(end2 - end1);
}


// Arcs
arc::arc(double centerX, double centerY, double radius, double theta0, double theta1) {
    center.x = centerX;
    center.y = centerY;

    this->radius = radius;
    this->theta0 = theta0;
    this->theta1 = theta1;
}

void arc::draw() const {
    int n;
    n = 60; // * (theta1 - theta0)/6;
//    std::cout << "The value of n is " << n << "\n";
    double *xCords = new double[n];
    double *yCords = new double[n];
    for (int i = 0; i < n; ++i) {
        xCords[i] = center.x + radius * cos(theta0 +  (theta1 - theta0) * i / (n-1));
        yCords[i] = center.y + radius * sin(theta0 +  (theta1 - theta0) * i / (n-1));
    }

    drawLine(xCords, yCords, n, 0, 0, 0);

    delete[] xCords;
    delete[] yCords;
}

vector arc::vectorToPoint(const vector &p) {
    double thetaPoint = atan2(p.y - center.y, p.x - center.x);
    if (thetaPoint < 0) thetaPoint += asin(1) * 2 * 2;
    vector origin;
    vector vToPoint;
//    printf("Angle = %f ", thetaPoint);

    if (theta0 < thetaPoint && thetaPoint < theta1) { // Checks if angle is inside arc.
        // get point on line
        double xTheta = cos(thetaPoint);
        double yTheta = sin(thetaPoint);
        origin.x = center.x + radius * xTheta;
        origin.y = center.y + radius * yTheta;
//        printf("\t xTheta = %f, yTheta = %f", xTheta, yTheta);
        // Get vector from line to arc
        vToPoint = p - origin;
    } else {
        // Calculate points for both ends of arc
        vector vEnd0(center.x + radius * cos(theta0), center.y + radius * sin(theta0));
        vector vEnd1(center.x + radius * cos(theta1), center.y + radius * sin(theta1));

        double distance = norm(p - vEnd0);
        if (norm(p - vEnd1) < distance) {
            // Point is closer to second end of arc;
            vToPoint = p - vEnd1;
            origin = vEnd1;
        } else {
            // Point is closer to first end of arc;
            vToPoint = p - vEnd0;
            origin = vEnd0;
        }
    }

    if (debugVectorDrawing) drawVector(origin, vToPoint, 250);
//    printf("\n");
    return vToPoint;
}
