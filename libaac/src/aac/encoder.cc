#include "aac/encoder.hh"

#include <iostream>

std::size_t aac::Encoder::step(Counter symbol, Byte* buffer,
                               std::size_t buffer_bitnum) {
    std::size_t bits_written { 0 };
    std::cout << "Encoding: " << std::hex << (int)symbol << std::endl;
    Range length { (Range)upper - (Range)lower + 1 };
    std::cout << "Range: [" << std::hex << lower << ", "
                            << upper
                            << "[" << std::endl;
    std::cout << "Length: " << std::dec << length << std::endl;
    Counter total_symbols { statistics.total() };
    std::cout << "Total symbols: " << std::dec << total_symbols << std::endl;
    Interval symbol_interval { statistics.symbol(symbol) };
    std::cout << "Symbol interval: [" << std::hex
                                      << symbol_interval.first << ", "
                                      << symbol_interval.second
                                      << "[" << std::endl;
    statistics.update(symbol); // Increment frequencies.
    std::cout << "Updated symbol frequencies." << std::endl;

    Bound rescale { ((Bound)length * (Bound)symbol_interval.second) };
    rescale /= (Bound)total_symbols;
    upper = lower + rescale - 1;

    rescale = ((Bound)length * (Bound)symbol_interval.first);
    rescale /= (Bound)total_symbols;
    lower = lower + rescale;

    std::cout << "Symbol range: [" << std::hex
                                   << lower << ", "
                                   << upper
                                   << "[" << std::endl;
    std::cout << "Symbol length: " << std::dec <<
                  upper - lower + 1 << std::endl;

    while (true) {
        // Matching most significant bit for lower and upper.
        if ((lower & SIGNIFICANT) == (upper & SIGNIFICANT)) {
            std::cout << "Matching most significant bit!" << std::endl;
            bool set { (upper & SIGNIFICANT) != 0x0000 };
            std::size_t head { buffer_bitnum+bits_written };
            std::cout << "Setting: " << set << " @ " << head << std::endl;
            bits_written += write(set, buffer, head);
        // Non-matching most significant bit and also non-matching
        // second most significant... We will never converge here.
        // Therefore, handle this case by letting some bit buffer.
        } else if ((lower & SECOND_MOST) && !(upper & SECOND_MOST)) {
            std::cout << "Underflow detected!" << std::endl;
            // Remove the SMSB from the stream...
            lower = lower & ~(SIGNIFICANT | SECOND_MOST);
            upper = upper | SECOND_MOST;
            std::cout << "Modified range: [" << std::hex
                << lower << ", "
                << upper
                << "[" << std::endl;
            ++buffered_bits;
            std::cout << "Bits buffered: " << buffered_bits << std::endl;
        // Everything is ok.
        } else break;

        lower = lower << 1;
        upper = upper << 1;
        upper = upper | 1;
    }

    return bits_written;
}

std::size_t aac::Encoder::exit(Byte* buffer, std::size_t buffer_bit) {
    std::size_t bits_written { step(EOF_SYMBOL, buffer, buffer_bit) };
    std::size_t remaining { write((lower & SECOND_MOST) != 0, buffer,
                                  buffer_bit + bits_written) };
    return bits_written + remaining;
}

std::size_t aac::Encoder::write(bool state, Byte* buffer,
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
