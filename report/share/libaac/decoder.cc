#include "aac/decoder.hh"

std::size_t aac::Decoder::step(Counter codew, Byte* buffer,
                               std::size_t buffer_bitnum) {
    std::size_t bits_written { 0 };
    Range length { (Range)upper - (Range)lower + 1 };
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
