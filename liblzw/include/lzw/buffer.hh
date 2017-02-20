#ifndef LZW_BUFFER_HH
#define LZW_BUFFER_HH

#include "lzw/definitions.hh"
#include <type_traits>

namespace lzw {
    template<typename T>
    class BufferData final {
    public:
        ~BufferData() { delete[] data; }
        BufferData(std::size_t size) : size { size } {
            data = new T[size];
        }

        BufferData() { // Ugly hack for getting "default" size for a buffer.
            if (std::is_same<T, Byte>::value)  size = lzw::WORD_BUFFER_SIZE;
            if (std::is_same<T, Index>::value) size = lzw::CODE_BUFFER_SIZE;
            data = new T[size];
        }

        T* data;
        std::size_t size;
    };

    using ByteBufferData = BufferData<Byte>;
    using IndexBufferData = BufferData<Index>;
    using CodeBufferData  = IndexBufferData;
    using WordBufferData  = ByteBufferData;

    template<typename T>
    class Buffer final {
    public:
        Buffer() = default;
        Buffer(BufferData<T>& data)
            : data { data.data},
              size { data.size } {  }
        Buffer(T* data,  std::size_t size)
            : data { data }, size { size } {  }

        // Utilities for manually controlling
        // the buffer's head/datum behaviour.
        void feed(T* data, std::size_t size);
        void trim(std::size_t location);
        void reset() { head = 0; }
        void ff(std::size_t amount);
        void rewind(std::size_t amount);
        T* rdbuf() { return data; }

        // Quality of life functions for observing
        // the behaviour of the byte/index buffer.
        bool last() const { return head >= size; }
        std::size_t length() const { return size; }
        std::size_t left() const { return size - head; }

        // The meat of the class, reads and writes bytes
        // or indices. Provides simple abstractions when
        // writing/reading large amount of data at once.
        T read(); // Increments the tape head forward...
        void write(T data); // Writes & increments head.
        void write(const T* data, std::size_t size);
        const T* read(std::size_t size);

    protected:
    private:
        T* data { nullptr };
        std::size_t size { 0 };
        std::size_t head { 0 };
    };

    template<typename T>
    void Buffer<T>::feed(T* data, std::size_t size) {
        this->data = data;
        this->size = size;
        this->reset();
    }

    template<typename T>
    void Buffer<T>::trim(std::size_t location) {
        this->size = size;
        if (head > size) {
            head = size;
        }
    }

    template<typename T> void Buffer<T>::ff(std::size_t amount) { head += amount; }
    template<typename T> void Buffer<T>::rewind(std::size_t amount) { head -= amount; }

    template<typename T> T Buffer<T>::read() { return data[head++]; }
    template<typename T> void Buffer<T>::write(T data) { this->data[head++] = data; }

    template<typename T>
    const T* Buffer<T>::read(std::size_t size) {
        const T* data { this->data + head };
        ff(size); // fast-forward the head.
        return data; // The user will here
        // handle data reading "manually".
    }

    template<typename T>
    void Buffer<T>::write(const T* data, std::size_t size) {
        std::memcpy(head, data, size);
        ff(size); // fast-forward!
    }

    using ByteBuffer  = Buffer<Byte>;
    using IndexBuffer = Buffer<Index>;
    using CodeBuffer  = IndexBuffer;
    using WordBuffer  = ByteBuffer;
}

#endif
