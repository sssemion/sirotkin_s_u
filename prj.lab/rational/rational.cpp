#include <rational/rational.h>
#include <iostream>
#include <cctype>

int gcd(int x, int y) {
    x = std::abs(x);
    y = std::abs(y);
    while (y != 0) {
        x %= y;
        std::swap(x, y);
    }
    return x;
}

int lcm(int x, int y) {
    return x * y / gcd(x, y);
}

Rational::Rational(int num)
    : Rational(num, 1)
{
}

Rational::Rational(int num, int denom)
{
    set(num, denom);
}

Rational& Rational::operator=(int rhs) {
    set(rhs, 1);
    return *this;
}

void Rational::set(int num, int denom)
{
    if (denom == 0) {
        throw std::invalid_argument("Division by zero");
    }
    numerator = num;
    denominator = denom;
    if (numerator != 0) {
        int k = gcd(numerator, denominator);
        numerator /= k;
        denominator /= k;
    }
    else {
        denominator = 1;
    }
    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }
}

bool Rational::operator==(const Rational &rhs) const {
    return numerator == rhs.numerator && denominator == rhs.denominator;
}

bool Rational::operator==(int rhs) const {
    return operator==(Rational(rhs));
}

bool operator==(int lhs, const Rational &rhs) {
    return rhs == lhs;
}

bool Rational::operator!=(const Rational &rhs) const {
    return !operator==(rhs);
}

bool Rational::operator!=(int rhs) const {
    return !operator==(rhs);
}

bool operator!=(int lhs, const Rational &rhs) {
    return rhs != lhs;
}

bool Rational::operator>(const Rational &rhs) const {
    return numerator * rhs.denominator > rhs.numerator * denominator;
}

bool Rational::operator>(int rhs) const {
    return operator>(Rational(rhs));
}

bool Rational::operator>=(const Rational &rhs) const {
    return operator>(rhs) || operator==(rhs);
}

bool Rational::operator>=(int rhs) const {
    return operator>=(Rational(rhs));
}

bool Rational::operator<(const Rational &rhs) const {
    return !operator>=(rhs);
}

bool Rational::operator<(int rhs) const {
    return !operator>=(rhs);
}

bool Rational::operator<=(const Rational &rhs) const {
    return !operator>(rhs);
}

bool Rational::operator<=(int rhs) const {
    return !operator>(rhs);
}

bool operator>(int lhs, const Rational &rhs) {
    return rhs < lhs;
}

bool operator>=(int lhs, const Rational &rhs) {
    return rhs <= lhs;
}

bool operator<(int lhs, const Rational &rhs) {
    return rhs > lhs;
}

bool operator<=(int lhs, const Rational &rhs) {
    return rhs >= lhs;
}

Rational& Rational::operator+=(const Rational &rhs) {
    int k = lcm(denominator, rhs.denominator);
    int new_num = numerator * (k / denominator) + rhs.numerator * (k / rhs.denominator);
    set(new_num, k);
    return *this;
}

Rational& Rational::operator+=(int rhs) {
    return operator+=(Rational(rhs));
}

Rational& Rational::operator-=(const Rational &rhs) {
    return operator+=(-rhs);
}

Rational& Rational::operator-=(int rhs) {
    return operator-=(Rational(rhs));
}

Rational& Rational::operator*=(const Rational &rhs) {
    int new_num = numerator * rhs.numerator;
    int new_denom = denominator * rhs.denominator;
    set(new_num, new_denom);
    return *this;
}

Rational& Rational::operator*=(int rhs) {
    return operator*=(Rational(rhs));
}

Rational& Rational::operator/=(const Rational &rhs) {
    int new_num = numerator * rhs.denominator;
    int new_denom = denominator * rhs.numerator;
    set(new_num, new_denom);
    return *this;
}

Rational& Rational::operator/=(int rhs) {
    return operator/=(Rational(rhs));
}

std::ostream& Rational::writeTo(std::ostream &ostrm) const {
    ostrm << numerator << slash << denominator;
    return ostrm;
}

std::istream& Rational::readFrom(std::istream &istrm) {
    char sep = 0;
    int num = 0, denom = 0;
    istrm >> num;
    istrm.get(sep);
    if (!std::isdigit(istrm.peek())) {
        istrm >> denom;
        istrm.setstate(std::ios::failbit);
    }
    else
        istrm >> denom;

    if (!istrm.fail()) {
        if (sep == Rational::slash) {
            set(num, denom);
        }
        else {
            istrm.setstate(std::ios_base::failbit);
        }
    }
    return istrm;
}

Rational& Rational::operator++() {
    *this += 1;
    return *this;
}

Rational Rational::operator++(int) {
    Rational t(*this);
    ++(*this);
    return t;
}

Rational& Rational::operator--() {
    *this -= 1;
    return *this;
}

Rational Rational::operator--(int) {
    Rational t(*this);
    --(*this);
    return t;
}

Rational operator+(const Rational &lhs, const Rational &rhs) {
    Rational res(lhs);
    res += rhs;
    return res;
}

Rational operator+(const Rational &lhs, int rhs) {
    return lhs + Rational(rhs);
}

Rational operator+(int lhs, const Rational &rhs) {
    return rhs + Rational(lhs);
}

Rational operator+(const Rational &lhs) {
    return lhs;
}

Rational operator-(const Rational &lhs, const Rational &rhs) {
    return lhs + (-rhs);
}

Rational operator-(const Rational &lhs, int rhs) {
    return lhs + (-rhs);
}

Rational operator-(int lhs, const Rational &rhs) {
    return -(rhs - lhs);
}

Rational operator-(const Rational &lhs) {
    return lhs * (-1);
}

Rational operator*(const Rational &lhs, const Rational &rhs) {
    Rational res(lhs);
    res *= rhs;
    return res;
}

Rational operator*(const Rational &lhs, int rhs) {
    return lhs * Rational(rhs);
}

Rational operator*(int lhs, const Rational &rhs) {
    return rhs * Rational(lhs);
}

Rational operator/(const Rational &lhs, const Rational &rhs) {
    Rational res(lhs);
    res /= rhs;
    return res;
}

Rational operator/(const Rational &lhs, int rhs) {
    return lhs / Rational(rhs);
}

Rational operator/(int lhs, const Rational &rhs) {
    return Rational(lhs) / rhs;
}


std::ostream& operator<<(std::ostream &ostrm, const Rational &rhs) {
    return rhs.writeTo(ostrm);
}

std::istream& operator>>(std::istream &istrm, Rational &rhs) {
    return rhs.readFrom(istrm);
}