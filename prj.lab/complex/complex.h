#pragma once
#ifndef SIROTKIN_S_U_COMPLEX_H
#define SIROTKIN_S_U_COMPLEX_H
#include <iosfwd>

struct Complex {
    Complex() = default;
    ~Complex() = default;
    explicit Complex(double real);
    Complex(double real, double imaginary);

    Complex& operator=(const Complex &rhs) = default;
    Complex& operator=(double rhs);

    bool operator==(const Complex &rhs) const;
    bool operator==(double rhs) const;
    bool operator!=(const Complex &rhs) const;
    bool operator!=(double rhs) const;

    Complex& operator+=(const Complex &rhs);
    Complex& operator+=(double rhs);

    Complex& operator-=(const Complex &rhs);
    Complex& operator-=(double rhs);

    Complex& operator*=(const Complex &rhs);
    Complex& operator*=(double rhs);

    Complex& operator/=(const Complex &rhs);
    Complex& operator/=(double rhs);

    std::ostream& writeTo(std::ostream& ostrm) const;
    std::istream& readFrom(std::istream& istrm);

    double re = 0, im = 0;

    static const char leftBracket = '{', separator = ',', rightBracket = '}';
};

Complex operator+(const Complex &lhs, const Complex &rhs);
Complex operator+(const Complex &lhs, double rhs);
Complex operator+(double lhs, const Complex &rhs);
Complex operator+(const Complex &lhs);

Complex operator-(const Complex &lhs, const Complex &rhs);
Complex operator-(const Complex &lhs, double rhs);
Complex operator-(double lhs, const Complex &rhs);
Complex operator-(const Complex &lhs);

Complex operator*(const Complex &lhs, const Complex &rhs);
Complex operator*(const Complex &lhs, double rhs);
Complex operator*(double lhs, const Complex &rhs);

Complex operator/(const Complex &lhs, const Complex &rhs);
Complex operator/(const Complex &lhs, double rhs);
Complex operator/(double lhs, const Complex &rhs);

bool operator==(double lhs, const Complex &rhs);
bool operator!=(double lhs, const Complex &rhs);


std::ostream& operator<<(std::ostream& ostrm, const Complex& rhs);
std::istream& operator>>(std::istream& istrm, Complex& rhs);

#endif //SIROTKIN_S_U_COMPLEX_H
