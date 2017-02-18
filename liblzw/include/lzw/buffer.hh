#ifndef LZW_BUFFER_HH
#define LZW_BUFFER_HH

#include "lzw/definitions.hh"

namespace lzw {
    class Buffer final {
    public:
        // Creates a buffer with size bytes.
        Buffer(Byte* data, std::size_t size)
            : data { data }, size { size } {  }

        // For manually controlling behaviour of the buffer.
        void rewind(std::size_t amount)  { head -= amount; }
        void skip(std::size_t amount) { head += amount; }
        void reset() { head = 0; } // The buffer start.

        // Utility functions for handling special cases.
        std::size_t left() const { return size - head; }
        std::size_t length() const { return size; }
        Byte* rdbuf() { return data; }
        void trim() { size = head; }

        // Useful set of functions for writing and reading bytes:
        Byte read_byte(); // Reads one byte from the buffer while
        // also pushing the buffer head/tape forward by one byte.
        Byte write_byte(Byte data); // Writes one byte to buffer,
        // while also incrementing the buffer head by one 8-byte.

        // Below follow some extra functions for the indices too.
        Index read_index(); // Auxiliary function for reading the
        // indices too, which are two bytes. Needed in the dicts.
        Index write_index(Index data); // And the write here too.

    private:
        Byte* data { nullptr };
        std::size_t size { 0 };
        std::size_t head { 0 };
    };
}

#endif
