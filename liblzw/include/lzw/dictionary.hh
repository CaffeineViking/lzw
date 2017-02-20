#ifndef LZW_DICTIONARY_HH
#define LZW_DICTIONARY_HH

#include "lzw/definitions.hh"
#include "lzw/buffer.hh"

namespace lzw {
    class Dictionary final {
    public:
        Dictionary(Byte* heads, Index* prefixes,
                   Index* rights, Index* lefts,
                   Index* roots, std::size_t size)
            : heads { heads }, prefixes { prefixes },
              rights { rights }, lefts { lefts },
              roots { roots }, size { size } {
                  reset();
              }

        bool full() const { return head == EMPTY_STRING; } // If so, prob reset dict.
        void initialize(); // Assigns the standard 8-bit alphabet to the first items.
        void reset(); // When dictionary is full, we want to clear/overwrite entires.
        Index insert(Index prefix, Byte head); // Will *not* link the tree structure!
        Index search(Index prefix, Byte head); // Searches AND inserts the entry only
        // if it doesn't exist, giving either the existing index or the newly created
        // one. However, this function will properly setup the binary tree structure.
        Byte* traverse(Byte* buffer, Index index) const; // Populates one byte string
        // produced after traversing the chain of prefixes and heads while outputting
        // the results to the temporary buffer. This buffer should be large enough...
        // NOTE: buffer will be the ending index of the buffer since we need to build
        // it backwards. Luckily, the first element in the buffer will be returned...

    protected:
    private:
        Index insert_root(Index prefix, Byte head);
        Index insert_into_tree(Index prefix, Byte head, Index root);
        Byte* heads { nullptr }; // Contains the parts after the <prefix>, the <head>.
        Index* prefixes { nullptr }; // Now needed since we need to know parent index.
        Index* rights { nullptr }, // Stores the right node of the binary search tree.
               *lefts { nullptr }, // Same deal here, but here we have the left nodes.
               *roots { nullptr }; // Finally, the root of the tree which is <prefix>.
        std::size_t size { 0 },
                    head { 0 }; // Enables easier insertion of new dictionary entires.
    };
}

#endif
