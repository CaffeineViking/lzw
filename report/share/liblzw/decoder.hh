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
        void restart() { previous_index = EMPTY_STRING;
                         previous_index_head = UNKNOWN_WORD; }

    protected:
    private:
        Dictionary& dictionary;
        Index previous_index { EMPTY_STRING };
        Byte previous_index_head { UNKNOWN_WORD };
    };
}

#endif
