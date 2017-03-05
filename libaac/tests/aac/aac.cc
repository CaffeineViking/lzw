#include "foreign/catch.hpp"
#include "aac/aac.hh"

#include <cstdio>
#include <cstddef>

TEST_CASE("example encoding", "[encoder]") {
    aac::Symbol symbols[] { 'b', 'b', 'b', 'b', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'a', 'a',
                            'b', 'b', 'b', 'b', 'b', 'a', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'b' };
    std::size_t symbol_count { sizeof(symbols) / sizeof(aac::Symbol) };

    std::size_t b { 0 };
    aac::Byte buffer[4096];
    aac::Encoder symbol_encoder;
    for (std::size_t i { 0 }; i < symbol_count; ++i) {
        std::size_t bits { symbol_encoder.step(symbols[i], buffer, b) };
        printf("(%lu,", bits); // Prints out how many bits written...
        if (bits != 0) printf("%.*s)", (int)(bits >> 3) + 1, buffer);
        printf("\n\n");
        b += bits;
    } printf("\n");
}

TEST_CASE("example decoding", "[decoder]") {
}
