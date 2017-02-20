#include "lzw/encoder.hh"

std::size_t lzw::Encoder::step(std::size_t amount) {
    std::size_t encoded { 0 };
    while (amount > 0) { // Consumed buffer words.
        Index latest_entry_index { dict.index() };
        head = words.read(); // Next byte to code.
        Index index { dict.search(prefix, head) };
        amount -= 1; // We consumed a single word.

        bool exists {latest_entry_index==dict.index()};
        if (exists) prefix = index; // Nothing added...
        else { // Here, search should have given entry.
            codes.write(prefix); // Write encoding.
            prefix = head; // Look-ahead of one.
            encoded += 1; // Codes generated.
            if (dict.full()) dict.reset();
        }
    }

    // Prepare for user...
    codes.rewind(encoded);
    return encoded;
}
