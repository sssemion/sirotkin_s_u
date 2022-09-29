#pragma once
#ifndef SIROTKIN_S_U_RATIONAL_H
#define SIROTKIN_S_U_RATIONAL_H
#include <iosfwd>

class Rational {
public:
    Rational() = default;
    ~Rational() = default;
    explicit Rational(int num);
    Rational(int num, int denom);

    Rational& operator=(const Rational &rhs) = default;
    Rational& operator=(int rhs);

    bool operator==(const Rational &rhs) const;
    bool operator==(int rhs) const;
    bool operator!=(const Rational &rhs) const;
    bool operator!=(int rhs) const;
    bool operator>(const Rational &rhs) const;
    bool operator>(int rhs) const;
    bool operator>=(const Rational &rhs) const;
    bool operator>=(int rhs) const;
    bool operator<(const Rational &rhs) const;
    bool operator<(int rhs) const;
    bool operator<=(const Rational &rhs) const;
    bool operator<=(int rhs) const;

    Rational& operator+=(const Rational &rhs);
    Rational& operator+=(int rhs);

    Rational& operator-=(const Rational &rhs);
    Rational& operator-=(int rhs);

    Rational& operator*=(const Rational &rhs);
    Rational& operator*=(int rhs);

    Rational& operator/=(const Rational &rhs);
    Rational& operator/=(int rhs);

    Rational& operator++();
    Rational operator++(int);

    Rational& operator--();
    Rational operator--(int);

    int num() const {return numerator;};
    int denom() const {return denominator;};

    std::ostream& writeTo(std::ostream &ostrm) const;
    std::istream& readFrom(std::istream &istrm);

private:
    void set(int num, int denom);

    int numerator = 0, denominator = 1;

    static const char slash = '/';

};

Rational operator+(const Rational &lhs, const Rational &rhs);
Rational operator+(const Rational &lhs, int rhs);
Rational operator+(int lhs, const Rational &rhs);
Rational operator+(const Rational &lhs);

Rational operator-(const Rational &lhs, const Rational &rhs);
Rational operator-(const Rational &lhs, int rhs);
Rational operator-(int lhs, const Rational &rhs);
Rational operator-(const Rational &lhs);

Rational operator*(const Rational &lhs, const Rational &rhs);
Rational operator*(const Rational &lhs, int rhs);
Rational operator*(int lhs, const Rational &rhs);

Rational operator/(const Rational &lhs, const Rational &rhs);
Rational operator/(const Rational &lhs, int rhs);
Rational operator/(int lhs, const Rational &rhs);

bool operator==(int lhs, const Rational &rhs);
bool operator!=(int lhs, const Rational &rhs);
bool operator>(int lhs, const Rational &rhs);
bool operator>=(int lhs, const Rational &rhs);
bool operator<(int lhs, const Rational &rhs);
bool operator<=(int lhs, const Rational &rhs);

std::ostream& operator<<(std::ostream &ostrm, const Rational &rhs);
std::istream& operator>>(std::istream &istrm, Rational &rhs);
#endif //SIROTKIN_S_U_RATIONAL_H
