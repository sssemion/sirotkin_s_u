#include <bitset/bitset.h>
#include <cmath>
#include <stdexcept>
#include <iostream>


Bitset::Bitset(const int64_t size, const bool filler)
{
    resize(size, filler);
}

Bitset::Bitset(const std::string &s) :
    Bitset(s.size(), false)
{
    for (int i = 0; i < size_; ++i) {
        if (s[s.size() - i - 1] == '0') {}
        else if (s[s.size() - i - 1] == '1') {
            data_[i / data_element_size_] |= ((decltype(data_)::value_type)1 << (i % data_element_size_));
        }
        else {
            throw std::invalid_argument("Bitset must consist of 0, 1 only");
        }
    }
}

Bitset::Bitset(const Bitset &copy) :
    Bitset(copy.size_)
{
    std::copy(copy.data_.begin(), copy.data_.end(), data_.begin());
}



int64_t Bitset::size() const {
    return size_;
}

void Bitset::resize(const int64_t size, const bool filler) {
    if (size < 0)
        throw std::invalid_argument("Bitset error: Bitset size can not be negative");

    data_.resize(std::ceil((double)size / data_element_size_),
                 filler ? (((decltype(data_)::value_type)1 << (data_element_size_ - 1)) | (((decltype(data_)::value_type)1 << (data_element_size_ - 1)) - 1)) : 0);
    if (size_ < size && size_ % data_element_size_ != 0) {
        decltype(data_)::value_type mask = (((decltype(data_)::value_type)1 << (data_element_size_ - 1)) - 1) << (size_ % data_element_size_);
        if (filler) // Fill unused bits of the last int32_t of data_ vector with 1
            data_[size_ / data_element_size_] |= mask;
        else // ... with 0
            data_[size_ / data_element_size_] &= (~mask);
    }
    size_ = size;
}

Bitset::BitHolder::BitHolder(const int64_t index, Bitset &bitset) :
    index_(index), bitset_(bitset)
{
}

Bitset::BitHolder::operator bool() const {
    decltype(Bitset::data_)::value_type mask = (decltype(data_)::value_type)1 << (index_ % Bitset::data_element_size_);
    return (bitset_.data_[index_ / Bitset::data_element_size_] & mask) != 0;
}

Bitset::BitHolder& Bitset::BitHolder::operator=(const bool rhs) {
    decltype(Bitset::data_)::value_type mask = (decltype(data_)::value_type)1 << (index_ % Bitset::data_element_size_);
    bitset_.data_[index_ / Bitset::data_element_size_] &= (~mask);
    if (rhs)
        bitset_.data_[index_ / Bitset::data_element_size_] ^= mask;
    return *this;
}

Bitset::BitHolder Bitset::operator[](const int64_t position) {
    if (position >= size_ || position < 0) {
        throw std::out_of_range("Bitset error: index out of range");
    }
    return BitHolder(position, *this);
}

bool Bitset::operator[](const int64_t position) const {
    if (position >= size_ || position < 0) {
        throw std::out_of_range("Bitset error: index out of range");
    }
    decltype(data_)::value_type mask = (decltype(data_)::value_type)1 << (position % Bitset::data_element_size_);
    return (data_[position / data_element_size_] & mask) != 0;
};


std::ostream& Bitset::writeTo(std::ostream &ostrm) const {
    for (int i = size_ - 1; i >= 0; --i) {
        decltype(data_)::value_type mask = (decltype(data_)::value_type)1 << (i % data_element_size_);
        ostrm << ((data_[i / data_element_size_] & mask) != 0 ? 1 : 0);
    }
    return ostrm;
}

std::istream& Bitset::readFrom(std::istream &istrm) {
    std::string s = "";
    while (istrm.peek() == '0' || istrm.peek() == '1') {
        s.push_back(istrm.get());
    }
    if (s.size() > 0) {
        *this = Bitset(s);
        istrm.clear(std::ios::goodbit);
    }
    else
        istrm.setstate(std::ios::failbit);
    return istrm;
}

Bitset& Bitset::operator=(const Bitset &rhs) {
    Bitset copy(rhs);
    size_ = copy.size_;
    data_ = copy.data_;
    return *this;
}

Bitset& Bitset::operator&=(const Bitset &rhs) {
    if (size_ != rhs.size_)
        throw std::logic_error("Bitset error: sizes of operands do not match");

    for (int i = 0; i < data_.size(); ++i)
        data_[i] &= rhs.data_[i];
    return *this;
}

Bitset& Bitset::operator|=(const Bitset &rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("Bitset error: sizes of operands do not match");
    }
    for (int i = 0; i < data_.size(); ++i)
        data_[i] |= rhs.data_[i];
    return *this;
}

Bitset &Bitset::operator^=(const Bitset &rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("Bitset error: sizes of operands do not match");
    }
    for (int i = 0; i < data_.size(); ++i)
        data_[i] ^= rhs.data_[i];
    return *this;
}

Bitset& Bitset::operator<<=(const int64_t rhs) {
    if (rhs < 0)
        throw std::invalid_argument("Bitset error: bitwise shift by negative count is undefined");
    if (rhs >= size_)
        return operator&=(Bitset(size_, 0));

    int64_t cluster_shift = rhs / data_element_size_;
    int64_t bit_shift = rhs % data_element_size_;

    for (int i = data_.size() - 1; i > cluster_shift; --i) {
        data_[i] = data_[i - cluster_shift];
        data_[i] <<= bit_shift;
        if (bit_shift)
            data_[i] |= (data_[i - cluster_shift - 1] >> (data_element_size_ - bit_shift));
    }
    data_[cluster_shift] = data_[0];
    data_[cluster_shift] <<= bit_shift;
    for (int i = 0; i < cluster_shift; ++i)
        data_[i] = 0;
    return *this;
}

Bitset& Bitset::operator>>=(const int64_t rhs) {
    if (rhs < 0)
        throw std::invalid_argument("Bitset error: bitwise shift by negative count is undefined");
    if (rhs >= size_)
        return operator&=(Bitset(size_, 0));

    int64_t cluster_shift = rhs / data_element_size_;
    int64_t bit_shift = rhs % data_element_size_;

    for (int i = 0; i < data_.size() - cluster_shift - 1; ++i) {
        data_[i] = data_[i + cluster_shift];
        data_[i] >>= bit_shift;
        if (bit_shift)
            data_[i] |= (data_[i + cluster_shift + 1] << (data_element_size_ - bit_shift));
    }
    data_[data_.size() - cluster_shift - 1] = data_[data_.size() - 1];
    data_[data_.size() - cluster_shift - 1] >>= bit_shift;
    for (int i = data_.size() - cluster_shift; i < data_.size(); ++i)
        data_[i] = 0;
    return *this;
}

bool Bitset::operator==(const Bitset &rhs) const {
    if (size_ != rhs.size_)
        return false;
    if (size_ == 0)
        return true;
    for (int i = 0; i < data_.size() - 1; ++i) {
        if (data_[i] != rhs.data_[i])
            return false;
    }
    decltype(data_)::value_type mask = ((decltype(data_)::value_type)1 << (size_ % data_element_size_)) - 1;
    if (size_ % data_element_size_ == 0)
        mask = -1;
    if ((data_.back() ^ rhs.data_.back()) & mask)
        return false;
    return true;
}

bool Bitset::operator!=(const Bitset &rhs) const {
    return !operator==(rhs);
}

Bitset Bitset::operator~() const {
    Bitset result(*this);
    for (int i = 0; i < result.data_.size() - 1; ++i)
        result.data_[i] = ~result.data_[i];
    if (result.size_ % data_element_size_ != 0)
        result.data_.back() ^= (((decltype(data_)::value_type)1 << (result.size_ % data_element_size_)) - 1);
    return result;
}

Bitset Bitset::operator&(const Bitset &rhs) const {
    return Bitset(*this).operator&=(rhs);
}

Bitset Bitset::operator|(const Bitset &rhs) const {
    return Bitset(*this).operator|=(rhs);
}

Bitset Bitset::operator^(const Bitset &rhs) const {
    return Bitset(*this).operator^=(rhs);
}

Bitset Bitset::operator<<(const int64_t rhs) const {
    return Bitset(*this).operator<<=(rhs);
}

Bitset Bitset::operator>>(const int64_t rhs) const {
    return Bitset(*this).operator>>=(rhs);
}

std::istream& operator>>(std::istream &istrm, Bitset &data) {
    return data.readFrom(istrm);
}

std::ostream& operator<<(std::ostream &ostrm, const Bitset &data) {
    return data.writeTo(ostrm);
}
