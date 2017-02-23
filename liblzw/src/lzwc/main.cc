#include <lzw/buffer.hh>
#include <lzw/dictionary.hh>
#include <lzw/encoder.hh>

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

    std::ofstream compressed { argv[2], std::ios::binary };
    if (!compressed) { // Failed to open file for writing.
        std::cerr << "File '" << argv[2]
                  << "' isn't valid."
                  << std::endl;
        return 1;
    }

    lzw::DictionaryData dictionary_data;
    lzw::CodeBufferData code_buffer_data;
    lzw::Dictionary dictionary { dictionary_data };
    lzw::CodeBuffer code_buffer { code_buffer_data };
    lzw::Encoder word_encoder { dictionary };

    char buffer[4096]; // Page size.
    file.read(buffer, sizeof(buffer));
    while (file.gcount() > 0) {
        std::streamsize words { file.gcount() };
        // Warning!: last element in buffer is the eof char!
        for (std::streamsize i { 0 }; i < words; ++i) {
            std::size_t codes { word_encoder.step(buffer[i], code_buffer) };
            if (codes) compressed.write((char*)code_buffer.rdhead(),
                                         codes * sizeof(lzw::Code));
            code_buffer.reset(); // Set buffer head back to 0.
        }

        file.read(buffer, sizeof(buffer));
    }

    // There might be a prefixes or head element left
    // in the buffer, therefore we flush the encoder.
    if (word_encoder.finish(code_buffer)) {
        compressed.write((char*)code_buffer.rdhead(),
                         1 * sizeof(lzw::Code));
    }

    return 0;
}
