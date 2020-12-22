//
// Created by Patrick on 2020-11-27.
//

#ifndef INC_2CARS2RACING_CUSTOMMATH_H
#define INC_2CARS2RACING_CUSTOMMATH_H

#include <cmath>

const double PI = asin(1) * 2;

inline double square(const double &d) {
    return d * d; // Returns square of a number
}

/// Convert degrees to radians
inline double toRadians(const double &d) {
    return d * (2.0 * PI) / 360.0;
}

inline double mod(const double& d1, const double& d2) {
    double _d1 = d1;
    while (_d1 > d2) {
        _d1 -= d2;
    }
    return _d1;
}

#endif //INC_2CARS2RACING_CUSTOMMATH_H
