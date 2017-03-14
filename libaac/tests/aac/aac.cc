#include "foreign/catch.hpp"
#include "aac/aac.hh"

#include <iostream>

TEST_CASE("example encoding", "[encoder]") {
    aac::Symbol symbols[] { 'b', 'b', 'b', 'b', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'a', 'a',
                            'b', 'b', 'b', 'b', 'b', 'a', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'b' };
    std::size_t symbol_count { sizeof(symbols) / sizeof(aac::Symbol) };

    std::cout << std::dec;
    aac::Statistics statistics;
    aac::Encoder symbol_encoder { statistics };
    std::size_t buffer_bits { 0 }; aac::Byte buffer[aac::MAX_PENDING];
    for (std::size_t i { 0 }; i < sizeof(symbols) / sizeof(aac::Symbol); ++i)
        buffer_bits += symbol_encoder.step(symbols[i], buffer, buffer_bits);
    buffer_bits += symbol_encoder.exit(buffer, buffer_bits);

    buffer[buffer_bits >> 3] &= ~((1 << (8 - (buffer_bits % 8) + 1)) - 1);
    for (std::size_t i { 0 }; i < (buffer_bits >> 3); ++i) {
        std::cout << std::hex << (unsigned)buffer[i] << ' ';
    } std::cout << std::endl;
}

TEST_CASE("example decoding", "[decoder]") {
    aac::Codeword codes[] { 0x00, 0x62, 0x61, 0xff, 0x75, 0x09, 0x7d, 0x5f, 0x68, 0xbe, 0x3f,
                            0x96, 0x9a, 0x8c, 0x12, 0xfb, 0x7d, 0x26, 0xae, 0xc1, 0x9b };
    std::size_t code_count { sizeof(codes) / sizeof(aac::Codeword) };

    std::cout << std::dec;
    aac::Statistics statistics;
    aac::Decoder code_decoder { statistics };
    aac::Byte buffer[aac::MAX_PENDING];
    int buffer_bits { 0 };
    std::size_t i { 0 };
    int read { 0 };

    do {
        while (!code_decoder.fed())
            i += (code_decoder.feed(codes[i >> 3],
                                    i - 8*(i >> 3)));
        read = code_decoder.step(buffer, buffer_bits);
        buffer_bits += 8;
    } while (read != -1);

    for (std::size_t i { 0 }; i < (buffer_bits >> 3); ++i) {
        std::cout << std::hex << (unsigned)buffer[i] << ' ';
    } std::cout << std::endl;
}
