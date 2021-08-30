#include "BigInteger.h"

BigInteger::BigInteger(unsigned long long value) {
    this->_values.push_back(value);
}

/**
 * Using the theory of [binary long multiplication](https://en.wikipedia.org/wiki/Binary_multiplier)
 * @param a First BigInteger
 * @param b Second BigInteger
 * @return Product of a*b
 */
BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    while (b > 0) { // TODO b modified?
        // we're checking the lsb of b; we shift b until there's nothing left
        b >> 1;
    }
    return a;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
    if (a._values.size() != b._values.size()) return (a._values.size() > b._values.size()); // if a block it's 0 it gets removed, so having more blocks implies being greater

    // we iterate throughout all the blocks until one it's different (or we finish)
    for (size_t x = a._values.size()-1; x >= 0; x--) {
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

    result._values[0] >>= reminder; // the less bits are lost
    for (size_t x = 1; x < result._values.size(); x++) {
        // the upper bits must be shifted
        result._values[x-1] |= (result._values[x] & mask) << shift;
        result._values[x] >>= reminder;
    }
    return result;
}

std::ostream &operator<<(std::ostream &str, const BigInteger &v) {
    // TODO
    return str << std::to_string(v._values.at(0));
}