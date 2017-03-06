#ifndef AAC_DECODER_HH
#define AAC_DECODER_HH

#include "aac/configs.hh"
#include "aac/statistics.hh"

namespace aac {
    class Decoder final {
    public:
        Decoder() = default;
        Decoder(Statistics statistics)
            : statistics { statistics } {}

        // Decodes the given byte codewo
        // rd. Either produces some bits
        // or nil; writes to the buffer.
        // Returns the number of bits...
        std::size_t step(Counter, Byte*,
                         std::size_t b);
        // No need to terminate, we also
        // encoded the end of file byte.
        // However, we do need to locate
        // the symbol for a given range.

    private:
        std::size_t write(bool,   Byte*,
                         std::size_t b);
        Counter buffered_bits { 0 };
        Statistics statistics;
        Counter upper { UPPER_BOUND },
                lower { LOWER_BOUND };
    };
}

#endif
