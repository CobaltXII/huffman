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

// All Huffman tree nodes have a type (internal or leaf), a side (1 for left
// and 0 for right), and a frequency. The frequency of a Huffman tree node is
// the sum of the frequencies of the two child nodes for an internal node. For
// a leaf node, the frequency is the amount of times the leaf's glyph appears
// in the original plaintext.

struct huffman_tree_node
{
	huffman_tree_node_type type;

	int side;

	int frequency;
};

// A Huffman tree internal node contains two children.

struct huffman_tree_internal: huffman_tree_node
{
	huffman_tree_node* child_0;
	huffman_tree_node* child_1;

	huffman_tree_internal
	(
		huffman_tree_node* _child_0,
		huffman_tree_node* _child_1
	)
	{
		type = huffman_internal;

		child_0 = _child_0;
		child_1 = _child_1;

		frequency =
		(
			child_0->frequency +
			child_1->frequency
		);

		child_0->side = 1;
		child_1->side = 0;
	}
};

// A Huffman tree leaf node contains a glyph.

struct huffman_tree_leaf: huffman_tree_node
{
	char glyph;

	huffman_tree_leaf(char _glyph, int _frequency)
	{
		type = huffman_leaf;

		glyph = _glyph;

		frequency = _frequency;
	}
};
