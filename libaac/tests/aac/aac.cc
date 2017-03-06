#include "foreign/catch.hpp"
#include "aac/aac.hh"

#include <iostream>

TEST_CASE("example encoding", "[encoder]") {
    aac::Symbol symbols[] { 'b', 'b', 'b', 'b', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'a', 'a',
                            'b', 'b', 'b', 'b', 'b', 'a', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'b' };
    std::size_t symbol_count { sizeof(symbols) / sizeof(aac::Symbol) };

    aac::Encoder symbol_encoder;
    std::size_t buffer_bits { 0 };
    aac::Byte buffer[aac::MAX_PENDING];
    for (std::size_t i { 0 }; i < sizeof(symbols) / sizeof(aac::Symbol); ++i) {
        std::size_t b { symbol_encoder.step(symbols[i], buffer, buffer_bits) };
        std::cout << "Wrote: " << b << " bits" << std::endl << std::endl;
        buffer_bits += b;
    }

    std::size_t b { symbol_encoder.exit(buffer, buffer_bits) };
    std::cout << "Wrote: " << b << " bits" << std::endl << std::endl;
}

TEST_CASE("example decoding", "[decoder]") {
}
