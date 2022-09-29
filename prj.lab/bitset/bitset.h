#ifndef SIROTKIN_S_U_BITSET_H
#define SIROTKIN_S_U_BITSET_H
//#include <bitseti/bitseti.h>
#include <vector>
#include <iosfwd>
#include <string>

class Bitset {//: BitsetI {
public:
    class BitHolder {
    public:
        friend class Bitset;
        BitHolder(const int64_t index, Bitset &bitset);
        BitHolder& operator=(const bool rhs);
        ~BitHolder() = default;
        operator bool() const;

    private:
        int64_t index_ = 0;
        Bitset &bitset_;
    };

    explicit Bitset(const int64_t size = 0,
                    const bool filler = false);
    Bitset(const std::string &s);

    Bitset(const Bitset& copy);
    Bitset& operator=(const Bitset& rhs);
    Bitset& operator&=(const Bitset& rhs);
    Bitset& operator|=(const Bitset& rhs);
    Bitset& operator^=(const Bitset& rhs);
    Bitset& operator<<=(const int64_t rhs);
    Bitset& operator>>=(const int64_t rhs);
    ~Bitset() = default;
    bool operator==(const Bitset& rhs) const;
    bool operator!=(const Bitset& rhs) const;
    Bitset operator~() const;
    Bitset operator&(const Bitset& rhs) const;
    Bitset operator|(const Bitset& rhs) const;
    Bitset operator^(const Bitset& rhs) const;
    Bitset operator<<(const int64_t rhs) const;
    Bitset operator>>(const int64_t rhs) const;
    BitHolder operator[](const int64_t position);
    bool operator[](const int64_t position) const;
    int64_t size() const;
    void resize(const int64_t size, const bool filler = false);

//    virtual std::ostream& writeTo(std::ostream &ostrm) const override;
//    virtual std::istream& readFrom(std::istream &istrm) override;
    std::ostream& writeTo(std::ostream &ostrm) const;
    std::istream& readFrom(std::istream &istrm);

private:
    int64_t size_ = 0;
    std::vector<uint32_t> data_ = {};
    static const int data_element_size_ = sizeof(decltype(data_)::value_type) * 8;
};

std::ostream& operator<<(std::ostream &ostrm, const Bitset &data);
std::istream& operator>>(std::istream &istrm, Bitset &data);

#endif //SIROTKIN_S_U_BITSET_H
