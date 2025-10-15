#include "Vector.h"
#include <cmath>

using namespace VectorLibrary;

const Vector Vector::ZERO_VECTOR = Vector(Number(0), Number(0));
const Vector Vector::UNIT_VECTOR = Vector(Number(1), Number(1));

Vector::Vector(const Number& x_comp, const Number& y_comp): x_comp(x_comp), 
    y_comp(y_comp) {}

Vector::Vector(): x_comp(Number(0)), y_comp(Number(0)){}


Vector Vector::operator + (const Vector& other) const {
    return Vector(x_comp + other.x_comp, y_comp + other.y_comp);
}

Number Vector::operator * (const Vector& other) const {
    return x_comp * other.x_comp + y_comp * other.y_comp;
}

double Vector::getR() const {
    long double x = static_cast<long double>(x_comp.getRawNumber());
    long double y = static_cast<long double>(y_comp.getRawNumber());
    return std::sqrt(x * x + y * y);
}

double Vector::getPhi() const {
    double x = static_cast<long double>(x_comp.getRawNumber());
    double y = static_cast<long double>(y_comp.getRawNumber());

    if (x == 0.0 && y == 0.0) {
        return 0.0;
    }

    return std::atan2(y, x);
}