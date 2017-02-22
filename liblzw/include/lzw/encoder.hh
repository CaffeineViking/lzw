#ifndef LZW_ENCODER_HH
#define LZW_ENCODER_HH

#include "lzw/definitions.hh"
#include "lzw/dictionary.hh"
#include "lzw/buffer.hh"

namespace lzw {
    class Encoder final {
    public:
        Encoder(Dictionary& dictionary) : dictionary{ dictionary } {    }
        // Each iteration will consume 'amount' words from the word buffer and will
        // then output a certain returned amount of codewords out to the code buff.
        std::size_t step(Word word, CodeBuffer& code_buffer); // Step word encoder.
        void restart() { word_prefix = EMPTY_STRING; current_word = 0; } // Resets.
        std::size_t flush(CodeBuffer& code_buffer); // Write if anything is left...

    protected:
    private:
        Dictionary& dictionary;
        Word current_word { UNKNOWN_WORD };
        Index word_prefix { EMPTY_STRING };
    };
}

#endif
