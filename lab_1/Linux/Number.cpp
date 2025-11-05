#include "Number.h"
#include <stdexcept>
#include <cmath>
#include <limits>

using namespace NumberLibrary;

Number::Number(long long int number) : number(number) {}
Number::Number() : number(0) {}

long long int Number::getRawNumber() const {
    return number;
}

Number Number::operator + (const Number& other) const {
    if (willAdditionOverflow(number, other.number)) {
        throw std::overflow_error("Integer overflow in addition");
    }
    return Number(number + other.number);
}

Number Number::operator - (const Number& other) const {
    if (willSubtractionOverflow(number, other.number)) {
        throw std::overflow_error("Integer overflow in subtraction");
    }
    return Number(number - other.number);
}

Number Number::operator * (const Number& other) const {
    if (willMultiplicationOverflow(number, other.number)) {
        throw std::overflow_error("Multiplication overflow");
    }
    return Number(number * other.number);
}

Number Number::operator / (const Number& other) const {
    if (other.number == 0) {
        throw std::runtime_error("Division by zero");
    }
    if (willDivisionOverflow(number, other.number)) {
        throw std::overflow_error("Division overflow");
    }
    return Number(number / other.number);
}

Number Number::operator % (const Number& other) const {
    if (other.number == 0) {
        throw std::runtime_error("Division by zero in mod operation");
    }
    return Number(number % other.number);
}

Number Number::GCD(const Number& other) const {
    long long int a = std::abs(number);
    long long int b = std::abs(other.number);

    if (a == 0) return b;
    if (b == 0) return a;
    if (a == b) return a;

    while (b != 0) {
        long long int temp = b;
        b = a % b;
        a = temp;
    }

    return Number(a);
}

Number Number::LCM(const Number& other) const {
    if (number == 0 || other.number == 0) {
        return 0;
    }

    long long int a = std::abs(number);
    long long int b = std::abs(other.number);

    long long int gcd = GCD(other).getRawNumber();

    long long int temp = a / gcd;

    if (willMultiplicationOverflow(temp, b)) {
        throw std::overflow_error("Integer overflow in LCM calculation");
    }

    return Number(temp * b);
}

bool Number::willMultiplicationOverflow(long long int a, long long int b) {
    if (a > 0 && b > 0) {
        return a > std::numeric_limits<long long int>::max() / b;
    }
    if (a < 0 && b < 0) {
        return a < std::numeric_limits<long long int>::max() / b;
    }
    if (a < 0 && b > 0) {
        return a < std::numeric_limits<long long int>::min() / b;
    }
    if (a > 0 && b < 0) {
        return a > std::numeric_limits<long long int>::min() / b;
    }
    return false;
}

bool Number::willAdditionOverflow(long long int a, long long int b) {
    if (a > 0 && b > 0) {
        return a > std::numeric_limits<long long int>::max() - b;
    }
    else if (a < 0 && b < 0) {
        return a < std::numeric_limits<long long int>::min() - b;
    }
    return false;
}

bool Number::willSubtractionOverflow(long long int a, long long int b) {
    if (a >= 0 && b < 0) {
        return a > std::numeric_limits<long long int>::max() + b;
    }
    else if (a < 0 && b > 0) {
        return a < std::numeric_limits<long long int>::min() + b;
    }
    return false;
}

bool Number::willDivisionOverflow(long long int a, long long int b) {
    return a == std::numeric_limits<long long int>::min() && b == -1;
}





