#ifndef LZW_DEFINITIONS_HH
#define LZW_DEFINITIONS_HH

#include <cstddef>
#include <climits>
#include <cstring>

namespace lzw {
    // These are the default sizes of the word, and code buffers, which can still
    // be modified by the user, carefully... Since one code can generate several,
    // up to 65536 words maximum, we assume the worst case and allow only 1 code.
    static constexpr std::size_t WORD_BUFFER_SIZE { 2 << 16 }, // The worst case.
                                 CODE_BUFFER_SIZE { 1 }; // To be safe, only one.
    static constexpr std::size_t DICTIONARY_ITEMS { 2 << 16 }; // 65536 elements.
    static constexpr std::size_t BYTE_SIZE_IN_BITS { CHAR_BIT }, // Special case.
                                 ALPHABET_SIZE  { 1 << BYTE_SIZE_IN_BITS }; // A.
    using Byte = unsigned char; // Since the bitsize of characters isn't actually
    // guaranteed to be always 8 bits in all platforms, we fail to compile these.
    using Index = unsigned short; // According to the standard, shorts are always
    // 16-bits long. Therefore, we don't need to make any checks for this type...
    using String = Byte*; // Is used for storing the sequences in the dictionary.
    static constexpr std::size_t ENTRY_SIZE { sizeof(Byte) + 4 * sizeof(Index) };
    static_assert(BYTE_SIZE_IN_BITS == 8, "Your platform is an edge case. Sry.");
    static constexpr Index EMPTY_STRING = static_cast<Index>(DICTIONARY_ITEMS-1);
    using Word = Byte; using Code = Index; // Convenience for user's readability.
    static constexpr Code UNKNOWN_WORD { '?' }; // ??????????????????????????????
}

#endif
