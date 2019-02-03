# huffman

This is a file compressor and decompressor written in C++11. It is far from optimal, and uses around eight times more memory than required. However, it is still functional, and manages to halve the size of files in most cases. It can compress files very quickly, but decompression takes a bit longer.

# Building

huffman requires no external dependencies, besides the standard library. To build, just compile the single source file

```bash
clang++ huffman.cpp -o huffman.o -std=c++11
```

# Usage

The usage string is as follows, for those who prefer syntax rather than sentances

```
Usage: ./huffman.o <-e | -d> <path-to-input> <path-to-output>
```

To encrypt a file, use the -e flag along with the input and output paths; for example

```bash
./huffman.o -e test.txt test.txt.huff
```

To decrypt a file, use the -d flag along with the input and output paths; for example

```bash
./huffman.o -d test.txt.huff test.txt
```

