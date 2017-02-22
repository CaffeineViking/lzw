#include "lzw/lzw.hh"
#include "foreign/catch.hpp"

#include <iostream>

TEST_CASE("example usage") {
    lzw::Index codewords[] { 98, 256, 98, 97, 97, 257, 259,
                            262, 257, 256, 260, 263, 257 };
}

TEST_CASE("file encoding") {
}

TEST_CASE("file decoding") {
}
