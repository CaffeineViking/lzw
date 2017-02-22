#include "lzw/encoder.hh"

std::size_t lzw::Encoder::step(lzw::Word current_word, lzw::CodeBuffer& code_buffer) {
    Index freshest_string { dictionary.index() };
    Index current_string { dictionary.search(word_prefix, current_word) };
    bool string_exists { freshest_string == dictionary.index() };
    std::size_t codes_generated { 0 };

    // Here we see if the dictionary search resulted
    // in a new entry. If not we simply continue the
    // reading process until we find a unique string
    // and then inserting it to the dictionary. See:
    if (string_exists) word_prefix = current_string;
    else { // Below the string didn't exist in dict.
        code_buffer.write(word_prefix);
        word_prefix = current_word;
        // We require a new context
        // since our dictionary has
        // limited storage, it also
        // has the added benefit of
        // clearing unused strings.
        if (dictionary.full())
            dictionary.reset();
        codes_generated  +=  1;
    }

    // "Rewinds" the buffer for reading.
    code_buffer.rewind(codes_generated);
    return codes_generated;
}

std::size_t lzw::Encoder::finish(lzw::CodeBuffer& code_buffer) {
    std::size_t codes_generated { 0 };
    if (word_prefix != EMPTY_STRING) {
        code_buffer.write(word_prefix);
        codes_generated += 1;
    }

    // "Rewinds" the buffer for reading.
    code_buffer.rewind(codes_generated);
    return codes_generated;
}
