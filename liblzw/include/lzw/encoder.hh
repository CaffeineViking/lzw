#ifndef LZW_ENCODER_HH
#define LZW_ENCODER_HH

#include "lzw/definitions.hh"
#include "lzw/dictionary.hh"
#include "lzw/buffer.hh"

namespace lzw {
    class Encoder final {
    public:
        Encoder(WordBuffer& word_buffer, CodeBuffer& code_buffer, Dictionary& dictionary)
            : words { word_buffer }, codes { code_buffer }, dict { dictionary} {  }
        // Each iteration will consume 'amount' words from the word buffer and will
        // then output a certain returned amount of codewords out to the code buff.
        std::size_t step(std::size_t amount); // Reads buffer and writes codewords.
        void reset() { prefix = EMPTY_STRING; head = 0; } // Need to reset others??

    protected:
    private:
        Byte head { '?' };
        Index prefix { EMPTY_STRING };
        WordBuffer& words;
        CodeBuffer& codes;
        Dictionary&  dict;
    };
}

#endif
