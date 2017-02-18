#ifndef LZW_DEFINITIONS_HH
#define LZW_DEFINITIONS_HH

#include <cstdint>
#include <cstddef>
#include <climits>

namespace lzw {
    using Byte = unsigned char; // Might not be 8
    // bit, depending on the platform. Also there
    // is the issue with endianness, but we don't
    // handle this case in the library. The users
    // should then handle a edge-case themselves.
    constexpr std::size_t BYTE_BITS { CHAR_BIT };
}

#endif
