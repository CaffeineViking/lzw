#include "lzw/lzw.hh"
#include "foreign/catch.hpp"

#include <iostream>

TEST_CASE("example usage") {
    lzw::Code codewords[] { 298, 256, 98, 97, 97, 257, 259,
                               262, 257, 256, 260, 263 };
}

TEST_CASE("file encoding") {
}

TEST_CASE("file decoding") {
}
