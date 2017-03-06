#ifndef AAC_CONFIGS_HH
#define AAC_CONFIGS_HH

#include <utility>
#include <cstdint>
#include <cstddef>

namespace aac {
    using Byte = unsigned char;
    using Range = std::uint32_t;
    using Bound = std::uint32_t;
    using Symbol = unsigned char;
    using Counter = std::uint16_t;
    using Interval = std::pair<Counter, Counter>;
    static const std::size_t SYMBOLS { 257 };
    static const std::size_t MAX_PENDING { 32 };
    static const std::size_t END_OF_FILE { 256 };
    static const Counter EOF_SYMBOL { 256 };

    // We limit ourselves to 16-bit bounds because
    // we are using multiplication between bounds.
    static const Counter LOWER_BOUND {  0x0000  };
    static const Counter UPPER_BOUND {  0xFFFF  };
    static const Counter COUNT_BOUND {  0x3FFF  };
    // Maximum bound for frequencies (2 bit less).
    static const Counter SIGNIFICANT {  0x8000  };
    static const Counter SECOND_MOST {  0x4000  };

    // Here we define the bound limits for number convergance.
    static const Counter ONE_QUARTER { (UPPER_BOUND + 1) >> 2 };
    static const Counter TWO_QUARTERS { ONE_QUARTER << 1 };
    static const Counter THREE_QUARTERS { ONE_QUARTER + TWO_QUARTERS };
}

#endif
