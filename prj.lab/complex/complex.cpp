#include <complex/complex.h>
#include <iostream>

Complex::Complex(double real)
    : Complex(real, 0.0)
{
}

Complex::Complex(double real, double imaginary)
    : re(real), im(imaginary)
{
}

Complex &Complex::operator=(double rhs) {
    re = rhs;
    im = 0;
    return *this;
}

bool Complex::operator==(const Complex &rhs) const {
    return std::abs(re - rhs.re) < 2 * std::numeric_limits<double>::epsilon() &&
           std::abs(im - rhs.im) < 2 * std::numeric_limits<double>::epsilon();
}

bool Complex::operator==(double rhs) const {
    return operator==(Complex(rhs));
}

bool operator==(double lhs, const Complex &rhs) {
    return rhs == lhs;
}

bool operator!=(double lhs, const Complex &rhs) {
    return rhs != lhs;
}

bool Complex::operator!=(const Complex &rhs) const {
    return !Complex::operator==(rhs);
}

bool Complex::operator!=(double rhs) const {
    return !Complex::operator==(rhs);
}

Complex& Complex::operator+=(const Complex& rhs) {
    re += rhs.re;
    im += rhs.im;
    return *this;
}

Complex& Complex::operator+=(double rhs) {
    return operator+=(Complex(rhs));
}

Complex& Complex::operator-=(const Complex& rhs) {
    re -= rhs.re;
    im -= rhs.im;
    return *this;
}

Complex& Complex::operator-=(const double rhs) {
    return operator-=(Complex(rhs));
}

Complex& Complex::operator*=(const Complex& rhs) {
    double x = re * rhs.re - im * rhs.im;
    double y = re * rhs.im + im * rhs.re;
    re = x;
    im = y;
    return *this;
}

Complex& Complex::operator*=(const double rhs) {
    re *= rhs;
    im *= rhs;
    return *this;
}

Complex& Complex::operator/=(const Complex &rhs) {
    double new_re = (re * rhs.re + im * rhs.im) / (pow(rhs.re, 2) + pow(rhs.im, 2));
    double new_im = (rhs.re * im - re * rhs.im) / (pow(rhs.re, 2) + pow(rhs.im, 2));
    re = new_re;
    im = new_im;
    return *this;
}

Complex& Complex::operator/=(double rhs) {
    return operator/=(Complex(rhs));
}

Complex operator+(const Complex &lhs, const Complex &rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator+(const Complex &lhs, double rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator+(double lhs, const Complex &rhs) {
    return rhs + lhs;
}

Complex operator-(const Complex &lhs, const Complex &rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator-(const Complex &lhs, double rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator-(double lhs, const Complex &rhs) {
    return -(rhs - lhs);
}

Complex operator-(const Complex &lhs) {
    return Complex(lhs) * (-1);
}

Complex operator*(const Complex &lhs, const Complex &rhs) {
    Complex res(lhs);
    res *= rhs;
    return res;
}

Complex operator*(const Complex &lhs, double rhs) {
    Complex res(lhs);
    res *= rhs;
    return res;
}

Complex operator*(double lhs, const Complex &rhs) {
    return rhs * lhs;
}

Complex operator/(const Complex &lhs, const Complex &rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

Complex operator/(const Complex &lhs, double rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

Complex operator/(double lhs, const Complex &rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

std::ostream& operator<<(std::ostream& ostrm, const Complex& rhs) {
    return rhs.writeTo(ostrm);
}

std::istream& operator>>(std::istream& istrm, Complex& rhs) {
    return rhs.readFrom(istrm);
}

Complex operator+(const Complex &lhs) {
    return lhs;
}

std::ostream& Complex::writeTo(std::ostream &ostrm) const {
    ostrm << leftBracket << re << separator << im << rightBracket;
    return ostrm;
}

std::istream& Complex::readFrom(std::istream& istrm) {
    char left = 0, sep =0, right =0;
    double real = 0, imaginary = 0;
    istrm >> left >> real >> sep >> imaginary >> right;
    if (istrm.good()) {
        if (Complex::leftBracket == left &&
            Complex::separator == sep &&
            Complex::rightBracket == right)
        {
            re = real;
            im = imaginary;
        }
        else
            istrm.setstate(std::ios_base::failbit);
    }
    return istrm;
}
