#ifndef LZW_DECODER_HH
#define LZW_DECODER_HH

#include "lzw/definitions.hh"
#include "lzw/dictionary.hh"
#include "lzw/buffer.hh"

namespace lzw {
    class Decoder final {
    public:
        Decoder(Dictionary& dictionary) : dictionary { dictionary } {  }
        // Each iteration will consume, usually, a single code from the code buffer
        // and will then output an arbitrary (max 2^16 - 1) amount of words to buf.
        std::size_t step(Code code, WordBuffer& word_buffer); // Step word encoder.
        std::size_t initialize(Code code, WordBuffer& word_buffer); // Start state.
        void restart() { prev_prefix = EMPTY_STRING; } // Resets the decoder state.

    protected:
    private:
        Dictionary& dictionary;
        // No use to keep track of indices.
        Byte  prefix_head { UNKNOWN_WORD };
        Index prev_prefix { EMPTY_STRING };
    };
}

#endif
