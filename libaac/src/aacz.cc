#include "aac/encoder.hh"

#include <string>
#include <fstream>
#include <iostream>

int usage(const char* program) {
    std::cerr << "usage: "
              << program << " "
              << "<input-file> "
              << "<output-file>"
              << std::endl;
    return 1;
}

int main(int argc, char** argv) {
    if (argc != 3) return usage(argv[0]);
    std::string input_path  { argv[1] },
                output_path { argv[2] };

    // Only allow opening of existing input
    // files, but allow non-existing output.
    std::ifstream input_file { input_path,   std::ios::binary};
    if (!input_file)  return usage(argv[0]);
    std::ofstream output_file { output_path, std::ios::binary};
    if (!output_file) return usage(argv[0]);

    aac::Statistics default_statistics; // Uniform.
    aac::Encoder ascii_encoder { default_statistics };

    aac::Byte byte { 0 };
    std::size_t head { 0 };
    aac::Byte bit_buffer[aac::MAX_PENDING] { 0 };
    input_file.read(&byte, sizeof(aac::Byte));

    while (input_file.gcount()) {
        std::size_t bits_written { ascii_encoder.step(byte, bit_buffer, head) };
        head += bits_written; // We might have to delay them writing until full.
        if (head >> 3 != 0) { // That is, until we have written at least a byte.
            std::size_t bytes_written { head >> 3 };
            std::size_t bits_remaining { head - (bytes_written << 3) };
        }

        input_file.read(&byte, sizeof(aac::Byte));
    }

    return 0;
}
