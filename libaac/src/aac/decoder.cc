#include "aac/decoder.hh"

#include <iostream>

int aac::Decoder::step(Byte* buffer, std::size_t buffer_bitnum) {
    std::size_t bits_written { 0 };
    Range length { (Bound)upper - (Bound)lower + 1 };
    std::cout << "Number: " << std::hex << number << std::endl;
    std::cout << "Length: " << std::dec << length << std::endl;
    Bound unscaled { (Bound)number - (Bound)lower + 1 };
    unscaled *= statistics.total() - 1;
    unscaled /= length;

    std::cout << "Unscaled: " << std::hex << unscaled << std::endl;

    Counter symbol { statistics.find(unscaled) };
    std::cout << "Symbol: " << std::hex << symbol << std::endl;
    if (symbol==END_OF_FILE) return -1;
    buffer[buffer_bitnum >> 3] = symbol;

    Counter total_symbols { statistics.total() };
    std::cout << "Total symbols: " << std::dec << total_symbols << std::endl;
    Interval symbol_interval { statistics.symbol(symbol) };
    std::cout << "Symbols interval: " << std::dec << symbol_interval.first << ", "
                                                  << symbol_interval.second << std::endl;
    Bound rescale { ((Bound)length * (Bound)symbol_interval.second) };
    rescale /= (Bound)total_symbols;
    upper = lower + rescale - 1;

    rescale = ((Bound)length * (Bound)symbol_interval.first);
    rescale /= (Bound)total_symbols;
    lower = lower + rescale;
    statistics.update(symbol);

    std::cout << "Rescaled upper: " << std::hex << upper << std::endl;
    std::cout << "Rescaled lower: " << std::hex << lower << std::endl;

    while (true) {
        // Matching most significant bit for lower and upper.
        if ((lower & SIGNIFICANT) == (upper & SIGNIFICANT)) {
            std::cout << "Matching MSB" << std::endl;
        // Non-matching most significant bit and also non-matching
        // second most significant... We will never converge here.
        // Therefore, handle this case by letting some bit buffer.
        } else if ((lower & SECOND_MOST) && !(upper & SECOND_MOST)) {
            std::cout << "NON-Matching MSB" << std::endl;
            // Remove the SMSB from the stream...
            lower = lower & ~(SIGNIFICANT | SECOND_MOST);
            upper = upper | SECOND_MOST;
            number ^= SECOND_MOST;
        // Everything is ok.
        } else break;

        ++number_bits;
        ++bits_written;
        number = number << 1;
        lower = lower << 1;
        upper = upper << 1;
        upper = upper | 1;
    }

    std::cout << "Number in end: " << number << std::endl;
    std::cout << "Bits to load: " << std::dec << number_bits << std::endl;
    std::cout << "Read: " << bits_written << std::endl << std::endl;
    return bits_written;
}

std::size_t aac::Decoder::feed(Counter byte, std::size_t start) {
    std::cout << "Start: " << std::dec << start << std::endl;
    std::cout << "Byte: " << std::hex << byte << std::endl;
    std::size_t length { 8 - start };
    std::cout << "Byte length: " << std::dec << length << std::endl;
    std::size_t pending;
    if (length > number_bits) { pending = number_bits ; byte >>= (length - pending); }
    else { pending = length ;  byte <<= (number_bits - pending);}
    number |= byte;
    std::cout << "Number: " << std::hex << number << std::endl;
    number_bits -= pending;
    std::cout << "Bits left: " << std::dec << number_bits << std::endl << std::endl;
    return pending;
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
