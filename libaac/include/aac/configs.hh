#ifndef AAC_CONFIGS_HH
#define AAC_CONFIGS_HH

#include <utility>
#include <cstdint>
#include <cstddef>

namespace aac {
    using Byte = unsigned char;
    using Range = std::int32_t;
    using Bound = std::uint32_t;
    using Symbol = unsigned char;
    using Counter = std::uint32_t;
    using Interval = std::pair<Bound, Bound>;
    static const std::size_t SYMBOLS { 257 };
    static const std::size_t END_OF_FILE { 256 };
    static const Bound LOWER_BOUND { (0 << 16) - 0 };
    static const Bound UPPER_BOUND { (1 << 16) - 1 };
    static const Bound ONE_FOURTHS { 1 << (16 - 2) };
    static const Bound TWO_FOURTHS { 2*ONE_FOURTHS };
    static const Bound TRE_FOURTHS { 3*ONE_FOURTHS };
}

#endif
