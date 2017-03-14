#ifndef AAC_DECODER_HH
#define AAC_DECODER_HH

#include "aac/configs.hh"
#include "aac/statistics.hh"

namespace aac {
    class Decoder final {
    public:
        Decoder() = default;
        Decoder(Statistics statistics)
            : statistics { statistics } {
            upper = statistics.total();
        }

        // Decodes the given byte codewo
        // rd. Either produces some bits
        // or nil; writes to the buffer.
        // Returns the number of bits...
        int  step(Byte*, std::size_t b);
        // No need to terminate, we also
        // encoded the end of file byte.
        // However, we do need to locate
        // the symbol for a given range.

        // Feed some bits into the number.
        std::size_t feed(Counter, std::size_t);
        bool fed() { return number_bits == 0; }

    private:
        std::size_t write(bool,   Byte*,
                         std::size_t b);
        Counter buffered_bits { 0 };
        Statistics statistics;
        Counter number_bits { 16 };
        Counter number { 0x0000 };
        Counter upper { SYMBOLS },
                lower { 0 };
    };
}

#endif
