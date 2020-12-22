//
// Created by Patrick on 2020-10-30.
//

#ifndef SIDE_RACER_VECTOR_H
#define SIDE_RACER_VECTOR_H

#include <iostream>

struct vector {
    double x, y;

    vector() : x(0), y(0) {}

    vector(double x_, double y_) : x(x_), y(y_) {}

    vector(const vector &v) : x(v.x), y(v.y) {}

    vector operator+(const vector &v) const { return vector(x + v.x, y + v.y); }
    vector operator-(const vector &v) const { return vector(x - v.x, y - v.y); }

    vector operator+=(const vector &v){ return vector(x += v.x,y += v.y);}
    vector operator-=(const vector &v){ return vector(x -= v.x,y -= v.y);}

    vector operator+(const double &c) const { return vector(x + c, y + c); }
    vector operator-(const double &c) const { return vector(x - c, y - c); }
    vector operator*(const double &c) const { return vector(x * c, y * c); }
    vector operator/(const double &c) const { return vector(x / c, y / c); }

    vector operator+=(const double &c) { return vector(x+= c, y+= c); }
    vector operator-=(const double &c) { return vector(x-= c, y-= c); }
    vector operator*=(const double &c) { return vector(x*= c, y*= c); }
    vector operator/=(const double &c) { return vector(x/= c, y/= c); }
    vector operator=(const double &c) { return vector(x = c, y = c); }
};

struct Mat22 {
    vector column1, column2;
    Mat22() {}
    Mat22(const vector &col1, const vector &col2) : column1(col1),  column2(col2) {}
    Mat22(double angle) { ///< Rotation is in radians.
        double cosine = cos(angle);
        double sine = sin(angle);

        column1.x = cosine;     column2.x = - sine;
        column1.y = sine;       column2.y =   cosine;
    }
};

inline vector operator * (const double &c, const vector &vec){
    return vector(vec.x * c, vec.y * c);
}

inline vector operator * (const Mat22 &mat, const vector &vec) {
    return vector(vec.x * mat.column1.x + vec.y * mat.column2.x,
                  vec.x * mat.column1.y + vec.y * mat.column2.y);
}

inline double norm(const vector &v){
    return sqrt(v.x * v.x + v.y * v.y);
}

inline double distance(const vector &p1, const vector &p2){
    return norm(p1 - p2);
}
inline double cross(const vector &v1, const vector &v2){
    return v1.x * v2.y - v1.y * v2.x;
}

inline void zeroVector(vector &v){
    v.x = 0;
    v.y = 0;
}

inline Mat22 scaleMatrix(const double &scale){
    Mat22 scaleMatrix;
    scaleMatrix.column1.x = scale;
    scaleMatrix.column1.y = 0;
    scaleMatrix.column2.x = 0;
    scaleMatrix.column2.y = scale;
    return scaleMatrix;
}

inline double dotProduct(const vector &a, const vector &b){
    return a.x * b.x + a.y * b.y;
}

/// Projection of a onto b.
inline vector projection(const vector &a, const vector &b){
    return  b * (dotProduct(a, b) / dotProduct(b, b)); // returns projection of vector (http://sites.science.oregonstate.edu/math/home/programs/undergrad/CalculusQuestStudyGuides/vcalc/dotprod/img6.gif)
}

inline vector vectorRejection(const vector &a, const vector &b){
    return a - projection(a, b);  // https://wikimedia.org/api/rest_v1/media/math/render/svg/271fe0eb76362e80ace035c6dd02962597b90f49 & https://wikimedia.org/api/rest_v1/media/math/render/svg/bccb2a3d19cb6f83ea576405c42f6b460b9ac9ad
}

inline vector unit(const vector &vec){
    return vec / norm(vec);
}

inline void printVector(const vector &v){
    std::cout << "x: " << v.x << "  y: " << v.y << "\n";
}

#endif //SIDE_RACER_VECTOR_H