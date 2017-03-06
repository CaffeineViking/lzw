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
    aac::Byte bit_buffer[aac::MAX_PENDING];
    input_file.read((char*)&byte, sizeof(aac::Byte));

    while (input_file.gcount()) {
        std::cout << "Encoding: " << byte << std::endl;
        std::size_t bits_written { ascii_encoder.step(byte, bit_buffer, head) };
        std::cout << "Created: " << bits_written << " bits" << std::endl;
        head += bits_written; // We might have to delay writing them until full.
        std::cout << "Total: " << head << " bits" << std::endl;
        if ((head >> 3) != 0) { // That is, until we have written at least a byte.
            std::size_t bytes_written { head >> 3 };
            std::size_t bits_remaining { head - (bytes_written << 3) };
            std::cout << "Writing: " << bytes_written << " bytes" << std::endl;
            output_file.write((char*)bit_buffer, bytes_written);

            // Copy the remaining bits to head of the buffer.
            for (std::size_t i { 0 }; i < bits_remaining; ++i) {
                int b { (bit_buffer[bytes_written] >> (7 - i)) & 0x01 };
                bit_buffer[0] &= ~(1 << (7 - i));
                bit_buffer[0] |= b << (7 - i);
            }

            std::cout << "Bits remaining: " << bits_remaining << std::endl;
            std::cout << "Head of buffer: " << std::hex << (int)bit_buffer[0]
                                            << std::dec << std::endl;
            head = bits_remaining; // Next bits will be put after.
        }

        std::cout << std::endl;
        input_file.read((char*)&byte, sizeof(aac::Byte));
    }

    ascii_encoder.exit(bit_buffer, head);
    std::size_t bytes_written { head >> 3 };
    output_file.write((char*)bit_buffer, bytes_written);
    std::size_t bits_remaining { head - (bytes_written << 3) };

    if (bits_remaining > 0) { // Still stuff to copy.
        // Copy the remaining bits to head of the buffer.
        for (std::size_t i { 0 }; i < bits_remaining; ++i) {
            bool b { (bit_buffer[bytes_written] >> (7 - i)) & 0x01 };
            bit_buffer[0] &= ~(1 << (7 - i));
            bit_buffer[0] |= b << (7 - i);
        }

        // Finally, clear any padding bits added to bytes.
        for (std::size_t i { bits_remaining }; i < 7; ++i)
            bit_buffer[0] &= ~(1 << (7 - i));
        output_file.write((char*)bit_buffer, 1);
    }

    return 0;
}
