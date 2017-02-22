#include "lzw/decoder.hh"

#include <iostream>
#include <cstdio>

std::size_t lzw::Decoder::initialize(lzw::Code code, lzw::WordBuffer& word_buffer) {
    word_buffer.write(code);
    prefix_head = (Byte)code;
    prev_prefix = (Byte)code;
    return 1; // Always 1?
}

std::size_t lzw::Decoder::step(lzw::Code current_code, lzw::WordBuffer& word_buffer) {
    std::size_t words_generated { 0 };
    if (prev_prefix == EMPTY_STRING) {
        words_generated = initialize(current_code,
                                     word_buffer);
        word_buffer.rewind(words_generated);
        return words_generated;
    }

    if (dictionary.exists(current_code)) {
        lzw::Byte* buffer { word_buffer.rdbuf() };
        lzw::Byte* final_word { buffer + word_buffer.length() };
        lzw::Byte* first_word { dictionary.traverse(final_word,
                                               current_code) };

        long words_written { final_word - first_word };
        word_buffer.ff(word_buffer.length() - words_written);

        words_generated += words_written;
        lzw::Byte head { *first_word };
        prefix_head = head; // Easy look-up.
    } else {
        lzw::Byte* buffer { word_buffer.rdbuf() };
        lzw::Byte* final_word { buffer + word_buffer.length() };
        lzw::Byte* first_word { dictionary.traverse(final_word,
                                                prev_prefix) };
        lzw::Byte head { *first_word };

        long words_written { final_word - first_word };
        word_buffer.ff(word_buffer.length() - words_written);
        words_generated += words_written;
        word_buffer.write(prefix_head);
        words_generated += 1;
    }

    prev_prefix = current_code;
    word_buffer.rewind(words_generated);
    return words_generated;
}
