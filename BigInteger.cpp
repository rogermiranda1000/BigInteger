#include "BigInteger.h"

BigInteger::BigInteger(unsigned long long value) {
    this->_values.push_back(value);
}

BigInteger::BigInteger() {
    this->_values.push_back(0);
    this->_negative = false;
}

/**
 * Using the theory of [binary long multiplication](https://en.wikipedia.org/wiki/Binary_multiplier)
 * @param a First BigInteger
 * @param b Second BigInteger
 * @return Product of a*b
 */
BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    BigInteger result, cpyA = a, cpyB = b;
    while (cpyB > 0) {
        // we're checking the lsb of b; we shift b until there's nothing left

        if (cpyB._values[0] & 0x01) result += cpyA;

        cpyB >>= 1;
        cpyA <<= 1; // each bit of B "shifts" the addition
    }
    return result;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    // TODO negative
    BigInteger result, add;
    // pick the biggest one
    if (a._values.size() > b._values.size()) {
        result = a;
        add = b;
    }
    else {
        result = b;
        add = a;
    }

    bool overflow = false;
    size_t x;
    for (x = 0; x < add._values.size(); x++) {
        std::feclearexcept(FE_OVERFLOW);
        if (overflow) result._values[x]++;
        result._values[x] += add._values[x];
        overflow = (bool)std::fetestexcept(FE_OVERFLOW);
    }
    while (overflow) {
        std::feclearexcept(FE_OVERFLOW);
        if (result._values.size() < x) result._values[x]++;
        else result._values.push_back(1);
        overflow = (bool)std::fetestexcept(FE_OVERFLOW);
        x++;
    }
    return result;
}

BigInteger &BigInteger::operator+=(const BigInteger &a) {
    BigInteger tmp = (*this+a);
    this->_values = tmp._values;
    this->_negative = tmp._negative;
    return *this;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
    if (a._values.size() != b._values.size()) return (a._values.size() > b._values.size()); // if a block it's 0 it gets removed, so having more blocks implies being greater

    // we iterate throughout all the blocks until one it's different (or we finish)
    for (long x = a._values.size()-1; x >= 0; x--) {
        if (a._values[x] != b._values[x]) return (a._values[x] > b._values[x]);
    }
    return false; // they are equal
}

BigInteger operator>>(const BigInteger &a, unsigned long b) {
    BigInteger result = a;
    for (unsigned long x = 0; x < b/sizeof(unsigned long long); x++) {
        if (result._values.size() == 1) {
            // we can't go any further
            result._values[0] = 0;
            break;
        }

        result._values.pop_front(); // if we remove 64 bits it's like removing a whole block
    }

    unsigned short reminder = (unsigned short)(b % sizeof(unsigned long long)), // we need to shift the other bits
        mask = (1<<reminder)-1, // get the part that will be removed
        shift = sizeof(unsigned long long)-reminder; // if we need to calculate (blocks of 3 bits) '011 100 >> 2' the result should be '000 111', being the two ones from the upper part shifted only 1 place
    if (mask < 0) mask = 0;

    result._values[0] >>= reminder; // the less bits are lost
    for (size_t x = 1; x < result._values.size(); x++) {
        // the upper bits must be shifted
        result._values[x-1] |= (result._values[x] & mask) << shift;
        result._values[x] >>= reminder;
    }
    return result;
}

BigInteger &BigInteger::operator>>=(unsigned long a) {
    this->_values = (*this>>a)._values;
    return *this;
}

BigInteger operator<<(const BigInteger &a, unsigned long b) {
    BigInteger result = a;
    unsigned long n;
    for (n = 0; n < b/sizeof(unsigned long long); n++) result._values.push_front(0); // if we add 64 bits it's like adding a whole block

    unsigned short reminder = (unsigned short)(b % sizeof(unsigned long long)), // we need to shift the other bits
    shift = sizeof(unsigned long long)-reminder;

    result._values[result._values.size()-1] <<= reminder;
    for (long x = result._values.size()-2; x >= (long)n /* from 0 (if n > 0) to n-1 it's all 0 */; x--) {
        // the upper bits must be shifted
        result._values[x+1] |= result._values[x] >> shift;
        result._values[x] <<= reminder;
    }
    return result;
}

BigInteger &BigInteger::operator<<=(unsigned long a) {
    this->_values = (*this<<a)._values;
    return *this;
}

std::ostream &operator<<(std::ostream &str, const BigInteger &v) {
    // TODO
    return str << std::to_string(v._values.at(0));
}