#include "compress.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <bitset>

struct HuffmanNode {
    char ch;
    unsigned freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, unsigned f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

void buildHuffmanTree(const std::unordered_map<char, unsigned>& freqMap, HuffmanNode*& root) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
    for (const auto& entry : freqMap) {
        pq.push(new HuffmanNode(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }

    root = pq.top();
}

void generateCodes(HuffmanNode* node, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->ch] = code;
        return;
    }
    generateCodes(node->left, code + "0", codes);
    generateCodes(node->right, code + "1", codes);
}

void compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);

    if (!in) {
        std::cerr << "Error opening input file: " << inputFile << "\n";
        return;
    }

    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening output file: " << outputFile << "\n";
        return;
    }

    std::cout << "Compressing '" << inputFile << "' to '" << outputFile << "'...\n";

    std::unordered_map<char, unsigned> freqMap;
    char ch;
    while (in.get(ch)) {
        freqMap[ch]++;
    }

    HuffmanNode* root = nullptr;
    buildHuffmanTree(freqMap, root);

    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);

    unsigned codebookSize = codes.size();
    out.write(reinterpret_cast<char*>(&codebookSize), sizeof(codebookSize));
    for (const auto& entry : codes) {
        out.put(entry.first);
        unsigned len = entry.second.size();
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(entry.second.c_str(), len);
    }

    std::string encodedStr;
    in.clear();
    in.seekg(0);
    while (in.get(ch)) {
        encodedStr += codes[ch];
    }

    for (size_t i = 0; i < encodedStr.size(); i += 8) {
        std::bitset<8> bits(encodedStr.substr(i, 8).append(8 - (encodedStr.size() - i < 8 ? encodedStr.size() - i : 8), '0'));
        out.put(bits.to_ulong());
    }

    std::cout << "Compression complete.\n";
}
