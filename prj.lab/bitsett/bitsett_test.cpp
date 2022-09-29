#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <bitsett/bitsett.h>
#include "doctest/doctest.h"
#include <sstream>

TEST_SUITE("BitsetT testing") {
    TEST_CASE_TEMPLATE_DEFINE("Reading from a stream", T, test_case0) {
        BitsetT<T> bs(0);
        std::istringstream istrm("1101101191110011");
        istrm >> bs;
        CHECK(bs == BitsetT<T>("11011011"));
        istrm.str("11011011");
        istrm >> bs;
        CHECK(bs == BitsetT<T>("11011011"));
        istrm.str("111111000000111110000011110000111000110010");
        istrm >> bs;
        CHECK(bs == BitsetT<T>("111111000000111110000011110000111000110010"));

        bs = BitsetT<T>(0);
        istrm.str("");
        istrm >> bs;
        CHECK(istrm.fail());
        CHECK(bs == BitsetT<T>(0));

        istrm.str("287634589");
        istrm >> bs;
        CHECK(istrm.fail());
        CHECK(bs == BitsetT<T>(0));
    }

    TEST_CASE_TEMPLATE_DEFINE("Writing to a stream", T, test_case1) {
        std::ostringstream ostrm;
        ostrm << BitsetT<T>("1001011101101001101101001010");
        CHECK(ostrm.str() == "1001011101101001101101001010");
        ostrm.str("");

        ostrm << BitsetT<T>("0000000000000000000000000011");
        CHECK(ostrm.str() == "0000000000000000000000000011");
        ostrm.str("");

        ostrm << BitsetT<T>("");
        CHECK(ostrm.str() == "");
    }

    TEST_CASE_TEMPLATE_DEFINE("Initialization", T, test_case2) {
        SUBCASE("BitsetT<T>(const int64_t size = 0, const bool filler = false)") {
            std::ostringstream ostrm;
            ostrm << BitsetT<T>();
            CHECK(ostrm.str() == "");
            ostrm.str("");
            ostrm << BitsetT<T>(4);
            CHECK(ostrm.str() == "0000");
            ostrm.str("");
            ostrm << BitsetT<T>(4, 1);
            CHECK(ostrm.str() == "1111");
        }

        SUBCASE("BitsetT<T>(const std::string &s)") {
            std::ostringstream ostrm;
            ostrm << BitsetT<T>("110011010011010");
            CHECK(ostrm.str() == "110011010011010");
            ostrm.str("");
            ostrm << BitsetT<T>("");
            CHECK(ostrm.str() == "");
            ostrm.str("");
            ostrm << BitsetT<T>("00001000000000000000000000000000000000000000000000000000000001");
            CHECK(ostrm.str() == "00001000000000000000000000000000000000000000000000000000000001");
            CHECK_THROWS_WITH(BitsetT<T>("00110100101010010104010111010001"), "Bitset must consist of 0, 1 only");
        }

        SUBCASE("BitsetT<T>(const BitsetT<T>& copy)") {
            BitsetT<T> bs("11010011");
            BitsetT<T> bs_copy(bs);
            std::ostringstream ostrm1, ostrm2;
            ostrm1 << bs;
            ostrm2 << bs_copy;
            CHECK(bs == bs_copy);
            CHECK(ostrm1.str() == ostrm2.str());
            bs_copy[0] = 0;
            CHECK(bs != bs_copy);
        }
    }

    TEST_CASE_TEMPLATE_DEFINE("=", T, test_case3) {
        BitsetT<T> bs1("111000110010");
        BitsetT<T> bs2("000111001101");
        CHECK(bs1 != bs2);
        bs2 = bs1;
        CHECK(bs1 == bs2);
        bs2[0] = 1;
        CHECK(bs1 != bs2);
    }

    TEST_CASE_TEMPLATE_DEFINE("Bitholder, operator[]", T, test_case4) {
        const std::string s = "111000110010";
        BitsetT<T> bs(s);
        const BitsetT<T> bs_const(s);
        for (int i = 0; i < s.size(); ++i) {
            CHECK(bs[i] == s[s.size() - i - 1] - 48);
            CHECK(bs_const[i] == s[s.size() - i - 1] - 48);
        }
        bs[0] = 1;
        bs[2] = 1;
        bs[3] = 1;
        CHECK(bs == BitsetT<T>("111000111111"));
    }

    TEST_CASE_TEMPLATE_DEFINE("Bitwise operators", T, test_case5) {
        SUBCASE("&") {
            BitsetT<T> bs1("111000110010");
            BitsetT<T> bs2("110100110101");
            BitsetT<T> bs3 = bs1 & bs2;
            CHECK(bs3 == BitsetT<T>("110000110000"));
            bs1 &= bs2;
            CHECK(bs1 == bs3);
            CHECK(bs1 == (bs1 & bs3));
        }

        SUBCASE("|") {
            BitsetT<T> bs1("111000110010");
            BitsetT<T> bs2("110100110101");
            BitsetT<T> bs3 = bs1 | bs2;
            CHECK(bs3 == BitsetT<T>("111100110111"));
            bs1 |= bs2;
            CHECK(bs1 == bs3);
            CHECK(bs1 == (bs1 | bs3));
        }

        SUBCASE("^") {
            BitsetT<T> bs1("111000110010");
            BitsetT<T> bs2("110100110101");
            BitsetT<T> bs3 = bs1 ^ bs2;
            CHECK(bs3 == BitsetT<T>("001100000111"));
            bs1 ^= bs2;
            CHECK(bs1 == bs3);
            CHECK(BitsetT<T>(12) == (bs1 ^ bs3));
        }

        SUBCASE("~") {
            BitsetT<T> bs1("111000110010");
            BitsetT<T> bs2 = ~bs1;
            CHECK(bs2 == BitsetT<T>("000111001101"));
            CHECK(BitsetT<T>("000111001101") == ~bs1);
            CHECK(~bs2 == bs1);
        }

        SUBCASE(">>") {
            BitsetT<T> bs("11110000111000110010"); // size = 20
            CHECK((bs >> 1) == BitsetT<T>("01111000011100011001"));
            CHECK((bs >> 2) == BitsetT<T>("00111100001110001100"));
            CHECK((bs >> 10) == BitsetT<T>("00000000001111000011"));
            CHECK((bs >> 20) == BitsetT<T>(20));
            CHECK((bs >> 1000) == BitsetT<T>(20));
            CHECK_THROWS_WITH(bs >> -1, "Bitset error: bitwise shift by negative count is undefined");
            bs >>= 10;
            CHECK(bs == BitsetT<T>("00000000001111000011"));

            std::string s = "111111000000111110000011110000111000110010";
            bs = BitsetT<T>(s); // size = 42
            for (int i = 0; i < bs.size(); ++i) {
                bs >>= 1;
                s.pop_back();
                s = '0' + s;
                CHECK(bs == BitsetT<T>(s));
            }
        }

        SUBCASE("<<") {
            BitsetT<T> bs("11110000111000110010"); // size = 20
            CHECK((bs << 1) == BitsetT<T>("11100001110001100100"));
            CHECK((bs << 2) == BitsetT<T>("11000011100011001000"));
            CHECK((bs << 10) == BitsetT<T>("10001100100000000000"));
            CHECK((bs << 20) == BitsetT<T>(20));
            CHECK((bs << 1000) == BitsetT<T>(20));
            CHECK_THROWS_WITH(bs << -1, "Bitset error: bitwise shift by negative count is undefined");
            bs <<= 10;
            CHECK(bs == BitsetT<T>("10001100100000000000"));

            std::string s = "111111000000111110000011110000111000110010";
            bs = BitsetT<T>(s); // size = 42
            for (int i = 0; i < bs.size(); ++i) {
                bs <<= 1;
                s.push_back('0');
                s.erase(0, 1);
                CHECK(bs == BitsetT<T>(s));
            }
        }
    }

    TEST_CASE_TEMPLATE_DEFINE("size, resize methods", T, test_case6) {
        BitsetT<T> bs(16, 0);
        CHECK(bs.size() == 16);
        bs.resize(32, 1);
        CHECK(bs.size() == 32);
        CHECK(bs == BitsetT<T>("11111111111111110000000000000000"));
        bs.resize(48);
        CHECK(bs.size() == 48);
        CHECK(bs == BitsetT<T>("000000000000000011111111111111110000000000000000"));
        bs.resize(24);
        CHECK(bs.size() == 24);
        CHECK(bs == BitsetT<T>("111111110000000000000000"));
        bs.resize(0);
        CHECK(bs == BitsetT<T>());
    }

    typedef std::tuple<uint8_t, uint16_t, uint32_t, uint64_t> TYPES;
    TEST_CASE_TEMPLATE_APPLY(test_case0, TYPES);
    TEST_CASE_TEMPLATE_APPLY(test_case1, TYPES);
    TEST_CASE_TEMPLATE_APPLY(test_case2, TYPES);
    TEST_CASE_TEMPLATE_APPLY(test_case3, TYPES);
    TEST_CASE_TEMPLATE_APPLY(test_case4, TYPES);
    TEST_CASE_TEMPLATE_APPLY(test_case5, TYPES);
    TEST_CASE_TEMPLATE_APPLY(test_case6, TYPES);
}