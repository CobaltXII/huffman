#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
#include <vector>
#include <bitset>
#include <locale>
#include <map>

// A Huffman tree node may be an internal node or a leaf node. Internal nodes
// represent a connection between two Huffman tree nodes, and leaf nodes
// represent a glyph from the original plaintext's alphabet.

enum huffman_tree_node_type
{
	huffman_internal, huffman_leaf
};
