#ifndef LZW_DECODER_HH
#define LZW_DECODER_HH

#include "lzw/definitions.hh"
#include "lzw/dictionary.hh"
#include "lzw/buffer.hh"

namespace lzw {
    class Decoder final {
    public:
        Decoder(CodeBuffer& code_buffer, WordBuffer& word_buffer, Dictionary& dictionary)
            : codes { code_buffer }, words { word_buffer }, dict { dictionary} {  }
        // Each iteration will consume 'amount' codes from the word buffer and will
        // then output a certain returned amount of words out to the code buffer...
        std::size_t step(std::size_t amount); // Reads code buffer and write words.
        void reset() { prefix = EMPTY_STRING; head = 0; } // Need to reset others??

    protected:
    private:
        Byte head { '?' };
        Index prefix { EMPTY_STRING };
        CodeBuffer& codes;
        WordBuffer& words;
        Dictionary&  dict;
    };
}

#endif
