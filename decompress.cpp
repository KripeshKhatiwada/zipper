#include "decompress.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <bitset>

void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening input file.\n";
        return;
    }

    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening output file.\n";
        return;
    }

    std::cout << "Decompressing '" << inputFile << "' to '" << outputFile << "'...\n";

    unsigned codebookSize;
    in.read(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));

    std::unordered_map<std::string, char> codeToChar;
    for (unsigned i = 0; i < codebookSize; ++i) {
        char ch;
        in.get(ch);
        unsigned len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string code(len, ' ');
        in.read(&code[0], len);
        codeToChar[code] = ch;
    }

    std::string bitStr;
    char byte;
    while (in.get(byte)) {
        std::bitset<8> bits(byte);
        bitStr += bits.to_string();
    }

    std::string currentCode;
    for (char bit : bitStr) {
        currentCode += bit;
        if (codeToChar.count(currentCode)) {
            out.put(codeToChar[currentCode]);
            currentCode.clear();
        }
    }

    std::cout << "Decompression complete.\n";
}
