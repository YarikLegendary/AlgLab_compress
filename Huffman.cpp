#include "Huffman.h"

struct Node {

	unsigned char symbol;
	int freq;

	Node* left, *right;

	Node(unsigned char s, int f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}

	Node(int f, Node* l, Node* r) : symbol(0), freq(f), left(l), right(r) {}

	~Node() {
		delete left;
		delete right;
	}

};

struct MyCompare {
	bool operator()(Node* l, Node* r) const {
		return l->freq < r->freq;
	}
};

Node* buildHuffmanTree(const map<unsigned char, int>& freqs) {

	vector<Node*> nodes;
	for (auto& pair : freqs) {
		nodes.push_back(new Node(pair.first, pair.second));
	}

	while (nodes.size() > 1) {

		int min1 = 0;
		int min2 = 1;

		if (nodes[min2]->freq > nodes[min1]->freq) {
			swap(min1, min2);
		}

		for (int i = 2; i < nodes.size(); i++) {
			if (nodes[i]->freq < nodes[min1]->freq) {
				min2 = min1;
				min1 = i;
			}
			else if (nodes[i]->freq < nodes[min2]->freq) {
				min2 = i;
			}
		}

		Node* left = nodes[min1];
		Node* right = nodes[min2];
		Node* parent = new Node(left->freq + right->freq, left, right);

		if (min1 > min2) {
			nodes.erase(nodes.begin() + min1);
			nodes.erase(nodes.begin() + min2);
		}
		else {
			nodes.erase(nodes.begin() + min2);
			nodes.erase(nodes.begin() + min1);
		}
		nodes.push_back(parent);
	}
	Node* root = nodes[0];

	return root;
}

void buildCode(Node* root, vector<bool>& code, map<unsigned char, vector<bool>>& table) {

	if (root->left != NULL) {
		code.push_back(0);
		buildCode(root->left, code, table);
	}
	if (root->right != NULL) {
		code.push_back(1);
		buildCode(root->right, code, table);
	}
	if (root->left == nullptr && root->right == nullptr) table[root->symbol] = code;

	if (!code.empty()) {
		code.pop_back();
	}
}

vector <unsigned char> encodeHuf(const vector<unsigned char>& data) {

	map<unsigned char, int> freqs; // Считаем частоты
	for (auto s : data) freqs[s]++;

	Node* root = buildHuffmanTree(freqs);

	map<unsigned char, vector<bool>> table; vector<bool> code;
	buildCode(root, code, table);

	vector <unsigned char> result;

	result.push_back(freqs.size());

	for (auto& pair : freqs) {
		result.push_back(pair.first);  // символ

		// частота (4 байта)
		int freq = pair.second;
		result.push_back((freq >> 24) & 0xFF);
		result.push_back((freq >> 16) & 0xFF);
		result.push_back((freq >> 8) & 0xFF);
		result.push_back(freq & 0xFF);
	}

	// 3. Сохраняем оригинальный размер данных (4 байта)
	uint32_t originalSize = data.size();
	result.push_back((originalSize >> 24) & 0xFF);
	result.push_back((originalSize >> 16) & 0xFF);
	result.push_back((originalSize >> 8) & 0xFF);
	result.push_back(originalSize & 0xFF);


	int count = 0; unsigned char buf = 0;

	for (int i = 0; i < data.size(); i++) {

		unsigned char sym = data[i];
		vector<bool> symCode = table[sym];

		for (int j = 0; j < symCode.size(); j++) {

			buf = buf | symCode[j] << (7 - count);
			count++;

			if (count == 8) { count = 0; result.push_back(buf); buf = 0; }
		}
	}

	if (count > 0) { // Последний байт
		result.push_back(buf);
	}

	return result;
}

vector <unsigned char> decodeHuf(const vector<unsigned char>& compressed) {


	int pos = 0;

	// Читаем количество символов
	int numSymbols = compressed[pos++];

	// Восстанавливаем частоты
	map<unsigned char, int> freqs;
	for (int i = 0; i < numSymbols; i++) {
		unsigned char symbol = compressed[pos++];
		int freq = (compressed[pos] << 24) | (compressed[pos + 1] << 16) | (compressed[pos + 2] << 8) | compressed[pos + 3];
		pos += 4;
		freqs[symbol] = freq;
	}

	// Читаем оригинальный размер
	int originalSize = (compressed[pos] << 24) | (compressed[pos + 1] << 16) | (compressed[pos + 2] << 8) | compressed[pos + 3];
	pos += 4;

	// Строим дерево
	Node* root = buildHuffmanTree(freqs);

	vector <unsigned char> result;
	Node* cur = root;
    int count = 0;

    for (size_t i = pos; i < compressed.size() && result.size() < originalSize; ) {

        unsigned char byte = compressed[i];
        
        bool b = byte & (1 << (7 - count));
        
        if (b) cur = cur->right;

        else cur = cur->left;
            
		if (!cur->left && !cur->right) {
			result.push_back(cur->symbol);
			cur = root;
		}
        
        count++;
        if (count == 8) { count = 0; i++; }

    }
	return result;
}


