#include <complex/complex.h>
#include <iostream>
#include <sstream>
#include <cassert>

bool testParse(const std::string &s, const Complex &correct) {
    std::istringstream istrm(s);
    Complex n(0);
    istrm >> n;
    return istrm.good() && n == correct;
}

bool testWrite(const Complex &n, const std::string &correct) {
    std::ostringstream ostrm;
    ostrm << n;
    return ostrm.str() == correct;
}

int main() {
    int i = 0;

    // Input
    assert(testParse("{9.8,1}", Complex(9.8, 1))); ++i;
    assert(testParse("{ 9.8 ,   1 }", Complex(9.8, 1))); ++i;
    assert(testParse("{1,9.8}", Complex(1, 9.8))); ++i;
    assert(!testParse("{9; 1}", Complex(9, 1))); ++i;
    // Output
    assert(testWrite(Complex(7.77, 8.88), "{7.77,8.88}")); ++i;
    assert(!testWrite(Complex(-7, 8), "{-8,8}")); ++i;
    // Assignment (=)
    Complex z;
    z = 1.8;
    assert(z == Complex(1.8)); ++i;
    z = Complex(1.8, -1);
    assert(z == Complex(1.8, -1)); ++i;
    // Equality (==, !=)
    assert(Complex(1.3, 0) == Complex(1.3)); ++i;
    assert(Complex(1.3, 0) == 1.3); ++i;
    assert(1.3 == Complex(1.3)); ++i;
    assert(Complex(1.3, -1) != Complex(1.3)); ++i;
    assert(Complex(1.3, -1) != 1.3); ++i;
    assert(1.3 != Complex(1.3, -1)); ++i;
    // Addition (+)
    assert(Complex(3, 5.55) + Complex(4.44, 6) == Complex(7.44, 11.55)); ++i;
    assert(Complex(-3, 5.55) + Complex(4.44, -6) == Complex(1.44, -0.45)); ++i;
    assert(Complex(-3, 5.55) + Complex(4.44, -6) == Complex(1.44, -0.45 + std::numeric_limits<double>::epsilon())); ++i;
    assert(Complex(-3, 5.55) + Complex(4.44, -6) != Complex(1.44, -0.45 + 3 * std::numeric_limits<double>::epsilon())); ++i;
    // Subtraction (-)
    assert(Complex(3, 5.55) - Complex(4.44, 6) == Complex(-1.44, -0.45)); ++i;
    assert(Complex(-3, 5.55) - Complex(4.44, -6) == Complex(-7.44, 11.55)); ++i;
    // Multiplication (*)
    assert(Complex(3, 5) * Complex(10, 4) != Complex(30, 20)); ++i;
    assert(Complex(3, 5) * Complex(10, 4) == Complex(10, 62)); ++i;
    // Division (/)
    assert(Complex(1, 4) / Complex(1, 2) == Complex(1.8, 0.4)); ++i;
    assert(Complex(1, 4) / 2 == Complex(0.5, 2)); ++i;
    assert(10 / Complex(1, 1) == Complex(5, -5)); ++i;
    // Unary operators (-a, +a)
    assert(-Complex(1, 4) == Complex(-1, -4)); ++i;
    assert(+Complex(1, 4) == Complex(1, 4)); ++i;

    std::cout << i << " tests passed successfully\n";
    return 0;
}