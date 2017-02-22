#include "lzw/lzw.hh"
#include "foreign/catch.hpp"

#include <iostream>

TEST_CASE("example usage") {
    lzw::Word words[] { 'b', 'b', 'b', 'b', 'a', 'a', 'b', 'b', 'b', 'a', 'a', 'a', 'a', 'a',
                        'b', 'b', 'b', 'b', 'b', 'a', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'b' };

    lzw::DictionaryData dictionary_data;
    lzw::CodeBufferData code_buffer_data;
    lzw::CodeBuffer code_buffer { code_buffer_data };
    lzw::Dictionary dictionary { dictionary_data };
    lzw::Encoder encoder { dictionary };

    for (std::size_t i { 0 }; i < sizeof(words); ++i) {
        if (encoder.step(words[i],   code_buffer)) {
            lzw::Code code { code_buffer.read() };
            code_buffer.reset(); // For more data!
            std::cout << code << ' ';
        }
    }

    if (encoder.flush(code_buffer)) {
        lzw::Code code { code_buffer.read() };
        std::cout << code << std::endl;
    }
}

TEST_CASE("file encoding") {
}

TEST_CASE("file decoding") {
}
