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
            : statistics { statistics } {}
        std::size_t exit(Byte*,std::size_t);

        // Encodes the given char symbol
        // and either produces some bits
        // or nil; writes to the buffer.
        // Returns the number of bits...
        std::size_t step(Symbol,  Byte*,
                         std::size_t b);

    private:
        std::size_t write(bool, Byte*,
                         std::size_t);
        Statistics statistics;
        Counter upper { UPPER_BOUND },
                lower { LOWER_BOUND };
        Counter pending_results { 0 };
    };
}

#endif
