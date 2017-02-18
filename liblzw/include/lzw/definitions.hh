#ifndef LZW_DEFINITIONS_HH
#define LZW_DEFINITIONS_HH

#include <cstddef>
#include <climits>

namespace lzw {
    static constexpr std::size_t BYTE_SIZE_IN_BITS { CHAR_BIT }; // Weird SPARCs.
    using Byte = unsigned char; // Since the bitsize of characters isn't actually
    // guaranteed to be always 8 bits in all platforms, we fail to compile these.
    using Index = unsigned short; // According to the standard, shorts are always
    // 16-bits long. Therefore, we don't need to make any checks for this type...
    using String = Byte*; // Is used for storing the sequences in the dictionary.
    static_assert(BYTE_SIZE_IN_BITS == 8, "Your platform is an edge case. Sry.");
}

#endif
