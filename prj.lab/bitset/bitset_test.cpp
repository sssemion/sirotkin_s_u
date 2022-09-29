#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <bitset/bitset.h>
#include "doctest/doctest.h"
#include <sstream>

bool testParse(std::string input, std::string result) {
    Bitset bs(0);
    std::istringstream istrm(input);
    istrm >> bs;
    return bs == Bitset(result) && (istrm.good());
}

bool testWrite(Bitset &bs, std::string result) {
    std::ostringstream ostrm;
    ostrm << bs;
    return ostrm.str() == result;
}

TEST_SUITE("Bitset testing") {
    TEST_CASE("Reading from a stream") {
        CHECK(testParse("1101101191110011", "11011011"));
        CHECK(testParse("11011011", "11011011"));
        CHECK(testParse("111111000000111110000011110000111000110010",
                        "111111000000111110000011110000111000110010"));

        Bitset bs(0);
        std::stringstream istrm("");
        istrm >> bs;
        CHECK(istrm.fail());
        CHECK(bs == Bitset(0));

        istrm.clear();
        istrm << "287634589";
        istrm >> bs;
        CHECK(istrm.fail());
        CHECK(bs == Bitset(0));
    }

    TEST_CASE("Writing to a stream") {
        CHECK(testWrite(Bitset("1001011101101001101101001010"), "1001011101101001101101001010"));
        CHECK(testWrite(Bitset("0000000000000000000000000011"), "0000000000000000000000000011"));
        CHECK(testWrite(Bitset(""), ""));
    }

    TEST_CASE("Initialization") {
        SUBCASE("Bitset(const int64_t size = 0, const bool filler = false)") {
            std::ostringstream ostrm;
            ostrm << Bitset();
            CHECK(ostrm.str() == "");
            ostrm.str("");
            ostrm << Bitset(4);
            CHECK(ostrm.str() == "0000");
            ostrm.str("");
            ostrm << Bitset(4, 1);
            CHECK(ostrm.str() == "1111");
        }

        SUBCASE("Bitset(const std::string &s)") {
            std::ostringstream ostrm;
            ostrm << Bitset("110011010011010");
            CHECK(ostrm.str() == "110011010011010");
            ostrm.str("");
            ostrm << Bitset("");
            CHECK(ostrm.str() == "");
            ostrm.str("");
            ostrm << Bitset("00001000000000000000000000000000000000000000000000000000000001");
            CHECK(ostrm.str() == "00001000000000000000000000000000000000000000000000000000000001");
            CHECK_THROWS_WITH(Bitset("00110100101010010104010111010001"), "Bitset must consist of 0, 1 only");
        }

        SUBCASE("Bitset(const Bitset& copy)") {
            Bitset bs("11010011");
            Bitset bs_copy(bs);
            std::ostringstream ostrm1, ostrm2;
            ostrm1 << bs;
            ostrm2 << bs_copy;
            CHECK(bs == bs_copy);
            CHECK(ostrm1.str() == ostrm2.str());
            bs_copy[0] = 0;
            CHECK(bs != bs_copy);
        }
    }

    TEST_CASE("=") {
        Bitset bs1("111000110010");
        Bitset bs2("000111001101");
        CHECK(bs1 != bs2);
        bs2 = bs1;
        CHECK(bs1 == bs2);
        bs2[0] = 1;
        CHECK(bs1 != bs2);
    }

    TEST_CASE("Bitholder, operator[]") {
        const std::string s = "111000110010";
        Bitset bs(s);
        const Bitset bs_const(s);
        for (int i = 0; i < s.size(); ++i) {
            CHECK(bs[i] == s[s.size() - i - 1] - 48);
            CHECK(bs_const[i] == s[s.size() - i - 1] - 48);
        }
        bs[0] = 1;
        bs[2] = 1;
        bs[3] = 1;
        CHECK(bs == Bitset("111000111111"));
    }

    TEST_CASE("Bitwise operators") {
        SUBCASE("&") {
            Bitset bs1("111000110010");
            Bitset bs2("110100110101");
            Bitset bs3 = bs1 & bs2;
            CHECK(bs3 == Bitset("110000110000"));
            bs1 &= bs2;
            CHECK(bs1 == bs3);
            CHECK(bs1 == (bs1 & bs3));
        }

        SUBCASE("|") {
            Bitset bs1("111000110010");
            Bitset bs2("110100110101");
            Bitset bs3 = bs1 | bs2;
            CHECK(bs3 == Bitset("111100110111"));
            bs1 |= bs2;
            CHECK(bs1 == bs3);
            CHECK(bs1 == (bs1 | bs3));
        }

        SUBCASE("^") {
            Bitset bs1("111000110010");
            Bitset bs2("110100110101");
            Bitset bs3 = bs1 ^ bs2;
            CHECK(bs3 == Bitset("001100000111"));
            bs1 ^= bs2;
            CHECK(bs1 == bs3);
            CHECK(Bitset(12) == (bs1 ^ bs3));
        }

        SUBCASE("~") {
            Bitset bs1("111000110010");
            Bitset bs2 = ~bs1;
            CHECK(bs2 == Bitset("000111001101"));
            CHECK(Bitset("000111001101") == ~bs1);
            CHECK(~bs2 == bs1);
        }

        SUBCASE(">>") {
            Bitset bs("11110000111000110010"); // size = 20
            CHECK((bs >> 1) == Bitset("01111000011100011001"));
            CHECK((bs >> 2) == Bitset("00111100001110001100"));
            CHECK((bs >> 10) == Bitset("00000000001111000011"));
            CHECK((bs >> 20) == Bitset(20));
            CHECK((bs >> 1000) == Bitset(20));
            CHECK_THROWS_WITH(bs >> -1, "Bitset error: bitwise shift by negative count is undefined");
            bs >>= 10;
            CHECK(bs == Bitset("00000000001111000011"));

            std::string s = "111111000000111110000011110000111000110010";
            bs = Bitset(s); // size = 42
            for (int i = 0; i < bs.size(); ++i) {
                bs >>= 1;
                s.pop_back();
                s = '0' + s;
                CHECK(bs == Bitset(s));
            }
        }

        SUBCASE("<<") {
            Bitset bs("11110000111000110010"); // size = 20
            CHECK((bs << 1) == Bitset("11100001110001100100"));
            CHECK((bs << 2) == Bitset("11000011100011001000"));
            CHECK((bs << 10) == Bitset("10001100100000000000"));
            CHECK((bs << 20) == Bitset(20));
            CHECK((bs << 1000) == Bitset(20));
            CHECK_THROWS_WITH(bs << -1, "Bitset error: bitwise shift by negative count is undefined");
            bs <<= 10;
            CHECK(bs == Bitset("10001100100000000000"));

            std::string s = "111111000000111110000011110000111000110010";
            bs = Bitset(s); // size = 42
            for (int i = 0; i < bs.size(); ++i) {
                bs <<= 1;
                s.push_back('0');
                s.erase(0, 1);
                CHECK(bs == Bitset(s));
            }
        }
    }

    TEST_CASE("size, resize methods") {
        Bitset bs(16, 0);
        CHECK(bs.size() == 16);
        bs.resize(32, 1);
        CHECK(bs.size() == 32);
        CHECK(bs == Bitset("11111111111111110000000000000000"));
        bs.resize(48);
        CHECK(bs.size() == 48);
        CHECK(bs == Bitset("000000000000000011111111111111110000000000000000"));
        bs.resize(24);
        CHECK(bs.size() == 24);
        CHECK(bs == Bitset("111111110000000000000000"));
        bs.resize(0);
        CHECK(bs == Bitset());
    }
}