#include "lzw/buffer.hh"

lzw::Byte lzw::Buffer::read_byte() {
    lzw::Byte byte { data[head] };
    skip(sizeof(lzw::Byte));
    return byte;
}

lzw::Byte lzw::Buffer::write_byte(lzw::Byte data) {
    lzw::Byte byte { this->data[head] };
    this->data[head] = data;
    skip(sizeof(lzw::Byte));
    return byte;
}

lzw::Index lzw::Buffer::read_index() {
    lzw::Index index { *(Index*)(data + head) };
    skip(sizeof(lzw::Index));
    return index;
}

lzw::Index lzw::Buffer::write_index(lzw::Index data) {
    lzw::Index index { *(Index*)(this->data + head) };
    *(Index*)(this->data + head) = data;
    skip(sizeof(lzw::Index));
    return index;
}
