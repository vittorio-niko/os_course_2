#include <iostream>
#include "Vector.h"
#include "Number.h"

using namespace VectorLibrary;

int main() {
    Number a(3), b(4);
    Number c(1), d(2);

    std::cout << "sum a + b = " << (a + b).getRawNumber() << std::endl;
    std::cout << "product a * b = " << (a * b).getRawNumber() << std::endl;

    Vector v1(a, b);
    Vector v2(c, d);

    Vector sum = v1 + v2;
    Number dot = v1 * v2;

    std::cout << "v1 length = " << v1.getR() << std::endl;
    std::cout << "v1 angle = " << v1.getPhi() << std::endl;
    std::cout << "dot product = " << dot.getRawNumber() << std::endl;

    return 0;
}

