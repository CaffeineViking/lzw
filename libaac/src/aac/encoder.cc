#include "aac/encoder.hh"

#include <iostream>

std::size_t aac::Encoder::step(Symbol symbol, Byte* buffer,
                               std::size_t coming_bit) {
    Range range = upper - lower;
    Interval interval { statistics.symbol(symbol) };
    Counter denominator { statistics.total() };
    statistics.update(symbol);

    upper = lower + (range * interval.second) / denominator;
    lower = lower + (range * interval.first) / denominator;

    std::size_t bits { 0 };
    while (true) {
        if (upper < UPPER_LIMIT) {
            bits += write(0, buffer, coming_bit + bits);
            lower = lower << 1;
            upper = upper << 1;
            upper |= 1;
        } else if (lower >= UPPER_LIMIT) {
            bits += write(1, buffer, coming_bit + bits);
            lower = lower << 1;
            upper = upper << 1;
            upper |= 1;
        } else if (lower   >= LOWER_COVER
                   && upper < UPPER_COVER) {
            ++pending_results;
            lower = lower << 1;
            lower &= LOWER_LIMIT;
            upper = upper << 1;
            upper |= UPPER_FOLLW;
        } else break;
    }

    lower = lower << 1;
    upper = upper << 1;
    upper |= 1;
    return bits;
}

std::size_t aac::Encoder::write(bool state, Byte* buffer,
                         std::size_t coming_bit) {
    std::size_t bits_written { 1 };
    std::size_t byte { coming_bit >> 3 };
    std::size_t bit { coming_bit - byte*8 };

    Byte data { buffer[byte] };
    data = data &  ~(1 << bit);
    data = data | state << bit;
    buffer[byte] = data;

    ++bits_written;
    ++coming_bit;

    while (pending_results--) {
        byte = coming_bit >> 3;
        bit = coming_bit - byte*8;

        data = buffer[byte];
        data = data &  ~(1 << bit);
        data = data | !state << bit;
        buffer[byte] = data;

        ++bits_written;
        ++coming_bit;
    } pending_results = 0;
    return bits_written;
}
