#include <lzw/buffer.hh>
#include <lzw/dictionary.hh>
#include <lzw/decoder.hh>

#include <iostream>
#include <fstream>

void usage(const char* executable) {
    std::cerr << "usage: " << executable << " "
              << "<file-path> <compressed-path>"
              << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    std::ifstream file { argv[1], std::ios::binary };
    if (!file) { // Failed to open file for reading.
        std::cerr << "File '" << argv[1]
                  << "' isn't valid."
                  << std::endl;
        return 1;
    }

    std::ofstream decompressed { argv[2], std::ios::binary };
    if (!decompressed) { // Failed to open file for writing.
        std::cerr << "File '" << argv[2]
                  << "' isn't valid."
                  << std::endl;
        return 1;
    }

    lzw::DictionaryData dictionary_data;
    lzw::WordBufferData word_buffer_data;
    lzw::Dictionary dictionary { dictionary_data };
    lzw::WordBuffer word_buffer { word_buffer_data };
    lzw::Decoder code_decoder { dictionary };

    char buffer[4096]; // Page size.
    file.read(buffer, sizeof(buffer));
    while (file.gcount() > 0) {
        std::streamsize bytes { file.gcount() };
        // Warning!: last element in buffer is (probably) the eof char?
        for (std::streamsize i { 0 }; i < bytes; i += sizeof(lzw::Code)) {
            lzw::Code code { *(lzw::Code*)(buffer + i) };
            std::size_t words { code_decoder.step(code, word_buffer) };
            if (words) decompressed.write((char*)word_buffer.rdhead(), words);
            word_buffer.reset(); // Set the buffer head back to zero.
        }

        file.read(buffer, sizeof(buffer));
    }

    return 0;
}
