
---

### ✅ 4. `zipper`

```markdown
# Zipper — C++ File Compressor

A basic command-line file compressor and decompressor written in C++.

## ⚙️ Features
- Compress and decompress text files
- Intended support for Huffman encoding
- Modular design (compress.cpp, decompress.cpp)

## 🚀 Usage
```bash
g++ main.cpp compress.cpp decompress.cpp -o zipper
./zipper compress input.txt output.bin
./zipper decompress output.bin recovered.txt
