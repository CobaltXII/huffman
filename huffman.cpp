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

// Traverse a Huffman tree until the path to a leaf node with the specified
// glyph is found. Follow the path (starting from the root), and write the
// side of each node (excluding the end node) as a character to the codeword.

bool get_binary_path(huffman_tree_node* node, char glyph, std::string& codeword)
{
	// Add the side of the node to the codeword.

	codeword.push_back(node->side + '0');

	// Check if the node is a leaf node. If so, check if it's glyph matches
	// the specified glyph. If so, return true to the caller.

	if (node->type == huffman_leaf)
	{
		if (((huffman_tree_leaf*)node)->glyph == glyph)
		{
			return true;
		}
	}
	else
	{
		// The node is an internal node. Recursively call get_binary_path on
		// the node's children.

		if 
		(
			get_binary_path(((huffman_tree_internal*)node)->child_0, glyph, codeword) ||
			get_binary_path(((huffman_tree_internal*)node)->child_1, glyph, codeword)
		)
		{
			return true;
		}
	}

	// Could not find any matches, remove the most recently written character
	// from the codeword and return false to the caller.

	codeword.pop_back();

	return false;
}

// Get the binary codeword of a glyph within a Huffman tree.

std::string get_codeword(huffman_tree_node* root, char glyph)
{
	std::string codeword;

	get_binary_path(root, glyph, codeword);

	return codeword.substr(1, codeword.length() - 1);
}

// Encode a plaintext using a codeword map generated using the plaintext's
// Huffman tree.

std::string encode_plaintext(std::map<char, std::string>& codewords, std::string& plaintext)
{
	std::string encoded;

	for (size_t i = 0; i < plaintext.size(); i++)
	{
		encoded += codewords[plaintext[i]];
	}

	return encoded;
}
