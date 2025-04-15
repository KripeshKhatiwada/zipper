#include <iostream>
#include "compress.h"
#include "decompress.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage:\n  compress <input_file> <output_file>\n  decompress <input_file> <output_file>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];

    if (mode == "compress") {
        compressFile(input, output);
    } else if (mode == "decompress") {
        decompressFile(input, output);
    } else {
        std::cerr << "Invalid mode. Use 'compress' or 'decompress'.\n";
        return 1;
    }

    return 0;
}
