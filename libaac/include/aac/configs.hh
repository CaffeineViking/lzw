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
    static const Bound LOWER_BOUND { 0x00000000 };
    static const Bound LOWER_LIMIT { 0x7FFFFFFF };
    static const Bound UPPER_LIMIT { 0x80000000 };
    static const Bound UPPER_BOUND { 0xFFFFFFFF };
    static const Bound LOWER_COVER { 0x40000000 };
    static const Bound UPPER_COVER { 0xC0000000 };
    static const Bound UPPER_FOLLW { 0x80000001 };
}

#endif
