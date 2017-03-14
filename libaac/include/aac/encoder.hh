#ifndef AAC_ENCODER_HH
#define AAC_ENCODER_HH

#include "aac/configs.hh"
#include "aac/statistics.hh"
#include <cstddef>

namespace aac {
    class Encoder final {
    public:
        Encoder() = default;
        Encoder(Statistics statistics)
            : statistics { statistics } {
            upper = statistics.total();
        }

        // Encodes the given char symbol
        // and either produces some bits
        // or nil; writes to the buffer.
        // Returns the number of bits...
        std::size_t step(Counter, Byte*,
                         std::size_t b);
        // Write out remaining bits ok??
        std::size_t exit(Byte*   buffer,
                         std::size_t b);

    private:
        std::size_t write(bool,   Byte*,
                         std::size_t b);
        Counter buffered_bits { 0 };
        Statistics statistics;
        Counter upper { SYMBOLS },
                lower { 0 };
    };
}

#endif
