#ifndef LZW_DICTIONARY_HH
#define LZW_DICTIONARY_HH

#include "lzw/definitions.hh"

namespace lzw {
    class Dictionary final {
    public:
        Dictionary(Byte* heads, Index* indices,
                   Index* rights, Index* lefts,
                   Index* roots, std::size_t size)
            : heads { heads }, indices { indices },
              rights { rights }, lefts { lefts },
              roots { roots }, size { size } {  }

    protected:
    private:
        Byte* heads { nullptr }; // Contains the parts after the <prefix>, the <head>.
        Index* indices { nullptr }; // Is needed since we need to know parent indices.
        Index* rights { nullptr }, // Stores the right node of the binary search tree.
               *lefts { nullptr }, // Same deal here, but here we have the left nodes.
               *roots { nullptr }; // Finally, the root of the tree which is <prefix>.
        std::size_t size { 0 },
                    head { 0 }; // Enables easier insertion of new dictionary entires.
    };
}

#endif
