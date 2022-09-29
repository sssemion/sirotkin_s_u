#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <rational/rational.h>
#include <iostream>
#include "doctest/doctest.h"

bool testParseCorrect(const std::string &s, const Rational &correct) {
    std::istringstream istrm(s);
    Rational n(0);
    istrm >> n;
    return (istrm.good() || istrm.eof()) && n == correct;
}

bool testParseIncorrect(const std::string &s) {
    std::istringstream istrm(s);
    Rational n(0);
    istrm >> n;
    return istrm.fail() && n == Rational(0);
}

bool testWrite(const Rational &n, const std::string &correct) {
    std::ostringstream ostrm;
    ostrm << n;
    return ostrm.str() == correct;
}

TEST_SUITE("Rational testing") {
    TEST_CASE("Reading from a stream") {
        CHECK(testParseCorrect("3/5", Rational(3, 5)));
        CHECK(testParseCorrect("-3/7 ", Rational(-3, 7)));
        CHECK(testParseCorrect("0/1 ", Rational(0)));
        CHECK(testParseCorrect("0/15 ", Rational(0)));

        CHECK_THROWS_WITH(testParseIncorrect("1/0"), "Division by zero");
        CHECK(testParseIncorrect("3 / 5"));
        CHECK(testParseIncorrect("3/ 5"));
        CHECK(testParseIncorrect("3 /5"));
        CHECK(testParseIncorrect("3,5"));
        CHECK(testParseIncorrect("3\\5"));
        CHECK(testParseIncorrect("3/-5"));
        CHECK(testParseIncorrect("{3/7}"));
        CHECK(testParseIncorrect("4.2/ 7"));
        CHECK(testParseIncorrect("[4.2/ 7]"));
        CHECK(testParseIncorrect("4.2/ 7}"));
    }

    TEST_CASE("Writing to a stream") {
        CHECK(testWrite(Rational(3, 5), "3/5"));
        CHECK(testWrite(Rational(-3, 7), "-3/7"));
        CHECK(testWrite(Rational(3, -5), "-3/5"));
        CHECK(testWrite(Rational(3, 6), "1/2"));
        CHECK(testWrite(Rational(15, 25), "3/5"));
    }

    TEST_CASE("Initialization") {
        CHECK(testWrite(Rational(), "0/1"));
        CHECK(testWrite(Rational(7), "7/1"));
        CHECK(testWrite(Rational(7, 2), "7/2"));
        CHECK(testWrite(Rational(-7, 2), "-7/2"));
        CHECK(testWrite(Rational(7, -2), "-7/2"));
        CHECK(testWrite(Rational(-7, -2), "7/2"));
        CHECK(testWrite(Rational(0, -2), "0/1"));
        CHECK(testWrite(Rational(14, 21), "2/3"));
        CHECK(testWrite(Rational(77, 21), "11/3"));
        CHECK_THROWS_WITH(Rational(7, 0), "Division by zero");
    }

    TEST_CASE("Arithmetic operations") {
        SUBCASE("Addition (+)") {
            CHECK(Rational(3, 5) + Rational(10, 4) == Rational(62, 20));
            CHECK(Rational(-3, 5) + Rational(10, 4) == Rational(38, 20));
            CHECK(Rational(1, 4) + Rational(1, 4) == Rational(1, 2));
            CHECK(Rational(3, 5) + 5 == Rational(28, 5));
            CHECK(5 + Rational(3, 5) == Rational(28, 5));
            Rational n(3, 5);
            n += Rational(10, 4);
            CHECK(n == Rational(62, 20));
            n += 5;
            CHECK(n == Rational(162, 20));
        }

        SUBCASE("Subtraction (-)") {
            CHECK(Rational(3, 5) - Rational(10, 4) == Rational(-38, 20));
            CHECK(Rational(3, 5) - Rational(-10, 4) == Rational(62, 20));
            CHECK(Rational(1, 4) - Rational(1, 4) == Rational(0));
            CHECK(5 - Rational(3, 5) == -Rational(-22, 5));
            Rational n(3, 5);
            n -= Rational(10, 4);
            CHECK(n == Rational(-38, 20));
            n -= 5;
            CHECK(n == Rational(-138, 20));
            CHECK(-n == Rational(138, 20));
        }

        SUBCASE("Multiplication (*)") {
            CHECK(Rational(3, 5) * Rational(10, 4) == Rational(30, 20));
            CHECK(Rational(3, 5) * Rational(-10, 4) == Rational(-30, 20));
            CHECK(Rational(3, 5) * 5 == Rational(15, 5));
            CHECK(5 * Rational(3, 5) == Rational(15, 5));
            Rational n(3, 5);
            n *= Rational(10, 4);
            CHECK(n == Rational(30, 20));
            n *= (-5);
            CHECK(n == Rational(-150, 20));
        }

        SUBCASE("Division (/)") {
            CHECK(Rational(3, 5) / Rational(10, 4) == Rational(12, 50));
            CHECK(Rational(3, 5) / Rational(-10, 4) == Rational(-12, 50));
            CHECK(Rational(3, 5) / 5 == Rational(3, 25));
            CHECK(5 / Rational(3, 5) == Rational(25, 3));
            Rational n(3, 5);
            n /= Rational(10, 4);
            CHECK(n == Rational(6, 25));
            CHECK_THROWS_WITH(n /= 0, "Division by zero");
            CHECK(n == Rational(6, 25));
        }

        SUBCASE("Increment / decrement (++/--)") {
            Rational n(3, 5);
            CHECK(++n == Rational(8, 5));
            CHECK(n++ == Rational(8, 5));
            CHECK(n == Rational(13, 5));

            CHECK(--n == Rational(8, 5));
            CHECK(n-- == Rational(8, 5));
            CHECK(n == Rational(3, 5));
        }
    }

    TEST_CASE("Comparison") {
        SUBCASE("Equality (=, !=)") {
            CHECK(Rational(3, 5) == Rational(3, 5));
            CHECK(Rational(25, 100) == Rational(1, 4));
            CHECK(Rational(-39, 52) == Rational(-3, 4));
            CHECK(Rational(15, 3) == 5);
            CHECK(5 == Rational(45, 9));

            CHECK(Rational(-39, 52) != Rational(3, 4));
            CHECK(Rational(1, 4) != Rational(1, 8));
        }

        SUBCASE("Greater (>, >=)") {
            CHECK(Rational(3, 4) > Rational(2, 3));
            CHECK(Rational(4, 3) > 1);
            CHECK(Rational(4, 3) - Rational(1, 3) >= 1);
            CHECK_FALSE(1 > Rational(4, 3));
            CHECK(1 >= Rational(3, 3));
        }

        SUBCASE("Less (<, <=)") {
            CHECK(Rational(3, 4) < Rational(4, 3));
            CHECK(-Rational(3, 4) < Rational(3, 4));
            CHECK(Rational(4, 5) + Rational(2, 10) <= 1);
            CHECK(1 < Rational(4, 3));
        }
    }
}