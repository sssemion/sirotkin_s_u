//
// Created by sssemion on 16.05.2022.
//

#ifndef SIROTKIN_S_U_BITSETT_H
#define SIROTKIN_S_U_BITSETT_H

#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>

template<typename ClusterType>
class BitsetT {
public:
    class BitHolder {
    public:
        friend class BitsetT;
        BitHolder(const int64_t index, BitsetT &bitset) : index_(index), bitset_(bitset) {};
        BitHolder& operator=(const bool rhs);
        ~BitHolder() = default;
        operator bool() const;

    private:
        int64_t index_ = 0;
        BitsetT &bitset_;
    };

    explicit BitsetT(const int64_t size = 0, const bool filler = false) { resize(size, filler); };
    BitsetT(const std::string &s);

    BitsetT(const BitsetT& copy);
    BitsetT& operator=(const BitsetT& rhs);
    BitsetT& operator&=(const BitsetT& rhs);
    BitsetT& operator|=(const BitsetT& rhs);
    BitsetT& operator^=(const BitsetT& rhs);
    BitsetT& operator<<=(const int64_t rhs);
    BitsetT& operator>>=(const int64_t rhs);
    ~BitsetT() = default;
    bool operator==(const BitsetT& rhs) const;
    bool operator!=(const BitsetT& rhs) const { return !operator==(rhs); };
    BitsetT operator~() const;
    BitsetT operator&(const BitsetT& rhs) const { return BitsetT<ClusterType>(*this).operator&=(rhs); };
    BitsetT operator|(const BitsetT& rhs) const { return BitsetT<ClusterType>(*this).operator|=(rhs); };
    BitsetT operator^(const BitsetT& rhs) const { return BitsetT<ClusterType>(*this).operator^=(rhs); };
    BitsetT operator<<(const int64_t rhs) const { return BitsetT<ClusterType>(*this).operator<<=(rhs); };
    BitsetT operator>>(const int64_t rhs) const { return BitsetT<ClusterType>(*this).operator>>=(rhs); };
    BitHolder operator[](const int64_t position);
    bool operator[](const int64_t position) const;
    int64_t size() const { return size_; };
    void resize(const int64_t size, const bool filler = false);

    std::ostream& writeTo(std::ostream &ostrm) const;
    std::istream& readFrom(std::istream &istrm);

private:
    int64_t size_ = 0;
    std::vector<ClusterType> data_ = {};
    static const int data_element_size_ = sizeof(ClusterType) * 8;
};

template<typename ClusterType>
typename BitsetT<ClusterType>::BitHolder &BitsetT<ClusterType>::BitHolder::operator=(const bool rhs) {
    ClusterType mask = (ClusterType)1 << (index_ % BitsetT<ClusterType>::data_element_size_);
    bitset_.data_[index_ / BitsetT<ClusterType>::data_element_size_] &= (~mask);
    if (rhs)
        bitset_.data_[index_ / BitsetT<ClusterType>::data_element_size_] ^= mask;
    return *this;
}

template<typename ClusterType>
BitsetT<ClusterType>::BitHolder::operator bool() const {
    ClusterType mask = (ClusterType)1 << (index_ % BitsetT<ClusterType>::data_element_size_);
    return (bitset_.data_[index_ / BitsetT<ClusterType>::data_element_size_] & mask) != 0;
}



template<typename ClusterType>
std::ostream& operator<<(std::ostream &ostrm, const BitsetT<ClusterType> &data) { return data.writeTo(ostrm); };
template<typename ClusterType>
std::istream& operator>>(std::istream &istrm, BitsetT<ClusterType> &data) { return data.readFrom(istrm); };

template<typename ClusterType>
BitsetT<ClusterType>::BitsetT(const std::string &s) :
        BitsetT<ClusterType>(s.size(), false)
{
    for (int i = 0; i < size_; ++i) {
        if (s[s.size() - i - 1] == '0') {}
        else if (s[s.size() - i - 1] == '1') {
            data_[i / data_element_size_] |= ((ClusterType)1 << (i % data_element_size_));
        }
        else {
            throw std::invalid_argument("Bitset must consist of 0, 1 only");
        }
    }
}

template<typename ClusterType>
BitsetT<ClusterType>::BitsetT(const BitsetT<ClusterType> &copy) :
        BitsetT<ClusterType>(copy.size_)
{
    std::copy(copy.data_.begin(), copy.data_.end(), data_.begin());
}

template<typename ClusterType>
void BitsetT<ClusterType>::resize(const int64_t size, const bool filler) {
    if (size < 0)
        throw std::invalid_argument("Bitset error: Bitset size can not be negative");

    data_.resize(std::ceil((double)size / data_element_size_),
                 filler ? (((ClusterType)1 << (data_element_size_ - 1)) | (((ClusterType)1 << (data_element_size_ - 1)) - 1)) : 0);
    if (size_ < size && size_ % data_element_size_ != 0) {
        ClusterType mask = (((ClusterType)1 << (data_element_size_ - 1)) - 1) << (size_ % data_element_size_);
        if (filler) // Fill unused bits of the last int32_t of data_ vector with 1
            data_[size_ / data_element_size_] |= mask;
        else // ... with 0
            data_[size_ / data_element_size_] &= (~mask);
    }
    size_ = size;
}

template<typename ClusterType>
BitsetT<ClusterType>& BitsetT<ClusterType>::operator=(const BitsetT<ClusterType> &rhs) {
    BitsetT<ClusterType> copy(rhs);
    size_ = copy.size_;
    data_ = copy.data_;
    return *this;
}

template<typename ClusterType>
BitsetT<ClusterType>& BitsetT<ClusterType>::operator&=(const BitsetT<ClusterType> &rhs) {
    if (size_ != rhs.size_)
        throw std::logic_error("Bitset error: sizes of operands do not match");

    for (int i = 0; i < data_.size(); ++i)
        data_[i] &= rhs.data_[i];
    return *this;
}

template<typename ClusterType>
BitsetT<ClusterType>& BitsetT<ClusterType>::operator|=(const BitsetT<ClusterType> &rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("Bitset error: sizes of operands do not match");
    }
    for (int i = 0; i < data_.size(); ++i)
        data_[i] |= rhs.data_[i];
    return *this;
}

template<typename ClusterType>
BitsetT<ClusterType>& BitsetT<ClusterType>::operator^=(const BitsetT<ClusterType> &rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("Bitset error: sizes of operands do not match");
    }
    for (int i = 0; i < data_.size(); ++i)
        data_[i] ^= rhs.data_[i];
    return *this;
}

template<typename ClusterType>
BitsetT<ClusterType>& BitsetT<ClusterType>::operator<<=(const int64_t rhs) {
    if (rhs < 0)
        throw std::invalid_argument("Bitset error: bitwise shift by negative count is undefined");
    if (rhs >= size_)
        return operator&=(BitsetT<ClusterType>(size_, 0));

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

template<typename ClusterType>
BitsetT<ClusterType>& BitsetT<ClusterType>::operator>>=(const int64_t rhs) {
    if (rhs < 0)
        throw std::invalid_argument("Bitset error: bitwise shift by negative count is undefined");
    if (rhs >= size_)
        return operator&=(BitsetT<ClusterType>(size_, 0));

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

template<typename ClusterType>
bool BitsetT<ClusterType>::operator==(const BitsetT<ClusterType> &rhs) const {
    if (size_ != rhs.size_)
        return false;
    if (size_ == 0)
        return true;
    for (int i = 0; i < data_.size() - 1; ++i) {
        if (data_[i] != rhs.data_[i])
            return false;
    }
    ClusterType mask = ((ClusterType)1 << (size_ % data_element_size_)) - 1;
    if (size_ % data_element_size_ == 0)
        mask = -1;
    if ((data_.back() ^ rhs.data_.back()) & mask)
        return false;
    return true;
}

template<typename ClusterType>
BitsetT<ClusterType> BitsetT<ClusterType>::operator~() const {
    BitsetT<ClusterType> result(*this);
    for (int i = 0; i < result.data_.size() - 1; ++i)
        result.data_[i] = ~result.data_[i];
    if (result.size_ % data_element_size_ != 0)
        result.data_.back() ^= (((ClusterType)1 << (result.size_ % data_element_size_)) - 1);
    return result;
}

template<typename ClusterType>
typename BitsetT<ClusterType>::BitHolder BitsetT<ClusterType>::operator[](const int64_t position) {
    if (position >= size_ || position < 0) {
        throw std::out_of_range("Bitset error: index out of range");
    }
    return BitHolder(position, *this);
}

template<typename ClusterType>
bool BitsetT<ClusterType>::operator[](const int64_t position) const {
    if (position >= size_ || position < 0) {
        throw std::out_of_range("Bitset error: index out of range");
    }
    ClusterType mask = (ClusterType)1 << (position % data_element_size_);
    return (data_[position / data_element_size_] & mask) != 0;
};

template<typename ClusterType>
std::ostream& BitsetT<ClusterType>::writeTo(std::ostream &ostrm) const {
    for (int i = size_ - 1; i >= 0; --i) {
        ClusterType mask = (ClusterType)1 << (i % data_element_size_);
        ostrm << ((data_[i / data_element_size_] & mask) != 0 ? 1 : 0);
    }
    return ostrm;
}

template<typename ClusterType>
std::istream& BitsetT<ClusterType>::readFrom(std::istream &istrm) {
    std::string s = "";
    while (istrm.peek() == '0' || istrm.peek() == '1') {
        s.push_back(istrm.get());
    }
    if (s.size() > 0) {
        *this = BitsetT<ClusterType>(s);
        istrm.clear(std::ios::goodbit);
    }
    else
        istrm.setstate(std::ios::failbit);
    return istrm;
}


#endif //SIROTKIN_S_U_BITSETT_H
