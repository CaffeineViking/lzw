#include "aac/decoder.hh"

#include <iostream>

std::size_t aac::Decoder::step(Counter codew, Byte* buffer,
                               std::size_t buffer_bitnum) {
    std::size_t bits_written { 0 };
    number = number << 8; number += codew;

    Range length { (Bound)upper - (Bound)lower + 1 };
    Bound unscaled { (Bound)number - (Bound)lower + 1 };
    unscaled *= statistics.total() - 1;
    unscaled /= length;

    Counter symbol { statistics.find(unscaled) };
    if (symbol==END_OF_FILE) return bits_written;
    buffer[buffer_bitnum << 3] = symbol;
    bits_written += 8;

    statistics.update(symbol);
    Counter total_symbols { statistics.total() };
    Interval symbol_interval { statistics.symbol(symbol) };
    Bound rescale { ((Bound)length * (Bound)symbol_interval.second) };
    rescale /= (Bound)total_symbols;
    upper = lower + rescale - 1;

    rescale = ((Bound)length * (Bound)symbol_interval.first);
    rescale /= (Bound)total_symbols;
    lower = lower + rescale;

    while (true) {
        // Matching most significant bit for lower and upper.
        if ((lower & SIGNIFICANT) == (upper & SIGNIFICANT)) {
        // Non-matching most significant bit and also non-matching
        // second most significant... We will never converge here.
        // Therefore, handle this case by letting some bit buffer.
        } else if ((lower & SECOND_MOST) && !(upper & SECOND_MOST)) {
            // Remove the SMSB from the stream...
            lower = lower & ~(SIGNIFICANT | SECOND_MOST);
            upper = upper | SECOND_MOST;
            number ^= SECOND_MOST;
        // Everything is ok.
        } else break;

        lower = lower << 1;
        upper = upper << 1;
        upper = upper | 1;
    }

    return bits_written;
}

std::size_t aac::Decoder::write(bool state, Byte* buffer,
                                std::size_t buffer_bit) {
    std::size_t bits_written { 0 };
    std::size_t byte { buffer_bit >> 3 };
    std::size_t bit_offset { buffer_bit - byte*8 };
    std::size_t bit { 7 - bit_offset };

    Byte data { buffer[byte] };
    data = data & ~(1 << bit);
    data = data | state << bit;
    buffer[byte] = data;
    ++bits_written;
    ++buffer_bit;

    while (buffered_bits--) {
        byte = buffer_bit >> 3;
        bit_offset = buffer_bit - byte*8;
        bit = 7 - bit_offset;

        data = buffer[byte];
        data = data & ~(1 << bit);
        data = data | !state << bit;
        buffer[byte] = data;
        ++bits_written;
        ++buffer_bit;
    }

    buffered_bits = 0;
    return bits_written;
}
