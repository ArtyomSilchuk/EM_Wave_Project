#include<iostream>
#include "physics/Vector3.h"
#include <cmath>


Vector3::Vector3(float x, float y, float z)
    : x(x), y(y), z(z) {
}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
    if (scalar != 0.0f) {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    return Vector3(0.0f, 0.0f, 0.0f);
}

float Vector3::length() const {
    return std::sqrt(x*x+y*y+z*z);
}

Vector3 Vector3::normalized() const {
    float len = length();
    if (len > 0.0f) {
        return Vector3(x/len, y/len, z/len);
    }
return Vector3(0.0f, 0.0f, 0.0f);
}


Vector3 operator*(float scalar, const Vector3& vector)
{
    return vector*scalar;
}

