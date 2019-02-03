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

// Encode a Huffman tree to a reference to a string.

void encode_huffman_to_string(huffman_tree_node* node, std::string& string)
{
	if (node->type == huffman_leaf)
	{
		string += "1";

		// Write the binary representation of the node's glyph to the output
		// binary string.

		std::bitset<8> glyph_bits(((huffman_tree_leaf*)node)->glyph);

		string += glyph_bits[0] + '0';
		string += glyph_bits[1] + '0';
		string += glyph_bits[2] + '0';
		string += glyph_bits[3] + '0';
		string += glyph_bits[4] + '0';
		string += glyph_bits[5] + '0';
		string += glyph_bits[6] + '0';
		string += glyph_bits[7] + '0';
	}
	else
	{
		string += "0";

		// Recursively encode the node's children to the same output binary
		// string.

		encode_huffman_to_string(((huffman_tree_internal*)node)->child_0, string);
		encode_huffman_to_string(((huffman_tree_internal*)node)->child_1, string);
	}
}

// Encode a Huffman tree to a new binary string, and return that binary
// string.

std::string encode_huffman(huffman_tree_node* root)
{
	std::string encoded;

	encode_huffman_to_string(root, encoded);

	return encoded;
}

// Decode a Huffman tree node from a binary string, and increment the
// used_bits counter for each bit read. For each leaf node, add the leaf
// node's glyph to glyphs.

huffman_tree_node* decode_huffman_node(std::string& binary, int& used_bits, std::vector<char>& glyphs)
{
	if (binary[used_bits++] == '1')
	{
		// Leaf node.

		std::bitset<8> glyph_bits;

		glyph_bits[0] = binary[used_bits++] - '0';
		glyph_bits[1] = binary[used_bits++] - '0';
		glyph_bits[2] = binary[used_bits++] - '0';
		glyph_bits[3] = binary[used_bits++] - '0';
		glyph_bits[4] = binary[used_bits++] - '0';
		glyph_bits[5] = binary[used_bits++] - '0';
		glyph_bits[6] = binary[used_bits++] - '0';
		glyph_bits[7] = binary[used_bits++] - '0';

		// Store the glyph.

		char glyph = glyph_bits.to_ullong();

		glyphs.push_back(glyph);

		// Create the leaf node.

		return new huffman_tree_leaf(glyph, -1);
	}
	else
	{
		// Internal node. Decode children recursively.

		huffman_tree_node* child_0 = decode_huffman_node(binary, used_bits, glyphs);
		huffman_tree_node* child_1 = decode_huffman_node(binary, used_bits, glyphs);

		// Create the internal node.

		return new huffman_tree_internal(child_0, child_1);
	}
}

// Decode a Huffman tree from a binary string, and remove the encoded Huffman
// tree from the start of the binary string.

huffman_tree_node* decode_huffman(std::string& binary, std::vector<char>& glyphs)
{
	int used_bits = 0;

	huffman_tree_node* root = decode_huffman_node(binary, used_bits, glyphs);

	binary.erase(0, used_bits);

	return root;
}

// Load a file as a string (do this one plaintext files only).

std::string load_file_as_string(std::string path)
{
	std::ifstream file = std::ifstream(path);

	if (!file.is_open())
	{
		std::cout << "Could not open the file \"" << path << "\"" << std::endl;

		exit(EXIT_FAILURE);
	}

	std::stringstream buffer;

	buffer << file.rdbuf();

	return buffer.str();
}

// Driver program.

int main(int argc, char** argv)
{
	// Make sure the argument count is correct.

	if (argc != 4)
	{
		std::cout << "Usage: " << argv[0] << " <-e | -d> <path-to-input> <path-to-output>" << std::endl;

		exit(EXIT_FAILURE);
	}

	// Parse the <-e | -d> argument.

	int mode = -1;

	if (std::string(argv[1]) == "-e")
	{
		mode = 1;
	}
	else if (std::string(argv[1]) == "-d")
	{
		mode = 2;
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " <-e | -d> <path-to-file> <path-to-output>" << std::endl;

		exit(EXIT_FAILURE);
	}

	// Parse the <path-to-input> argument.

	std::string path_to_input = std::string(argv[2]);

	// Parse the <path-to-output> argument.

	std::string path_to_output = std::string(argv[3]);

	// Load the file as a string.

	std::string plaintext = load_file_as_string(path_to_input);

	// Encrypt or decrypt the input file, based on the mode.

	if (mode == 1)
	{
		// Create a map that maps each character in the alphabet of the plaintext
		// to it's frequency.

		std::map<char, int> frequencies;

		for (size_t i = 0; i < plaintext.size(); i++)
		{
			frequencies[plaintext[i]]++;
		}

		// Make a vector of Huffman tree nodes. Populate this vector with one leaf
		// node for each entry in the frequency table.

		std::vector<huffman_tree_node*> nodes;

		for (auto const& frequency: frequencies)
		{
			nodes.push_back(new huffman_tree_leaf(frequency.first, frequency.second));
		}

		// Repeat until nodes contains only one Huffman node.

		while (nodes.size() > 1)
		{
			// Find the two nodes with the lowest frequencies.

			huffman_tree_node* small_0 = nullptr;
			huffman_tree_node* small_1 = nullptr;

			size_t small_0_idx;
			size_t small_1_idx;

			int lowest_frequency = std::numeric_limits<int>::max();

			// Find the node with the lowest frequency.

			for (size_t i = 0; i < nodes.size(); i++)
			{
				if (nodes[i]->frequency < lowest_frequency)
				{
					small_0 = nodes[i];

					small_0_idx = i;

					lowest_frequency = small_0->frequency;
				}
			}

			// Remove the node with the lowest frequency (small_0) from nodes.

			nodes.erase(nodes.begin() + small_0_idx);

			// Find the node with the second lowest frequency (excluding the node
			// with the lowest frequency).

			lowest_frequency = std::numeric_limits<int>::max();

			for (size_t i = 0; i < nodes.size(); i++)
			{
				if (nodes[i]->frequency < lowest_frequency)
				{
					small_1 = nodes[i];

					small_1_idx = i;

					lowest_frequency = small_1->frequency;
				}
			}

			// Remove the node with the second lowest frequency (small_1) from 
			// nodes.

			nodes.erase(nodes.begin() + small_1_idx);

			// Create a Huffman internal node linking small_0 and small_1, then
			// add that node to nodes.

			nodes.push_back(new huffman_tree_internal(small_0, small_1));
		}

		// Create a map that maps each character in the alphabet of the plaintext
		// to it's binary codeword.

		std::map<char, std::string> codewords;

		for (auto const& frequency: frequencies)
		{
			codewords[frequency.first] = get_codeword(nodes[0], frequency.first);
		}
	}
	else if (mode == 2)
	{
	}

	// Exit successfully.

	exit(EXIT_SUCCESS);
}