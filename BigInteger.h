#pragma once

#include <deque>
#include <string>

class BigInteger {
public:
    BigInteger(const BigInteger &cpy) = default;
    BigInteger(unsigned long long value);
    BigInteger(long long value) : BigInteger((unsigned long long)value) {
        // TODO check sign
    };
    BigInteger(int value) : BigInteger((long long)value) {};

    friend BigInteger operator*(const BigInteger &a, const BigInteger &b);
    friend bool operator>(const BigInteger &a, const BigInteger &b);
    friend BigInteger operator>>(const BigInteger &a, unsigned long b);
    friend std::ostream &operator<<(std::ostream &str, const BigInteger &v);

private:
    /**
     * From the less digit to the greatest.
     * Ex (12 splitted on groups of 2 bits): 1100; 11 00; stored: [00, 11]
     */
    std::deque<unsigned long long> _values;
    bool _negative; // TODO
};
