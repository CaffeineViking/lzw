#include "lzw/decoder.hh"

#include <iostream>

std::size_t lzw::Decoder::step(lzw::Code current_code,
                               lzw::WordBuffer& word_buffer) {
    std::size_t decoded_words { 0 };
    if (previous_index == EMPTY_STRING) {
        word_buffer.write(current_code);
        previous_index_head = current_code;
        previous_index = current_code;
        decoded_words += 1;
    } else {
        if (dictionary.exists(current_code)) {
            lzw::Byte* buffer { word_buffer.rdbuf() };
            lzw::Byte* end_buffer { buffer + word_buffer.length() };
            lzw::Byte* start_buffer { dictionary.traverse(end_buffer, current_code) };

            long int buffer_size { end_buffer - start_buffer };
            word_buffer.ff(word_buffer.length());

            decoded_words += buffer_size;
            previous_index_head = *start_buffer;
            dictionary.insert(previous_index, *start_buffer);
        } else {
            lzw::Index new_entry_index { dictionary.insert(previous_index, previous_index_head) };

            lzw::Byte* buffer { word_buffer.rdbuf() };
            lzw::Byte* end_buffer { buffer + word_buffer.length() };
            lzw::Byte* start_buffer { dictionary.traverse(end_buffer, new_entry_index) };

            long int buffer_size { end_buffer - start_buffer };
            word_buffer.ff(word_buffer.length());
            decoded_words += buffer_size;
            previous_index_head = *start_buffer;
        }

        previous_index = current_code;
        if (dictionary.full()) {
            dictionary.reset();
            this->restart();
        }
    }

    word_buffer.rewind(decoded_words);
    return decoded_words;
}
