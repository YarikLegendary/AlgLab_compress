#include "Huffman.h"

struct Node {

	unsigned char sym;
	int freq;

	Node* left;
	Node* right;

	Node(unsigned char s, int f) : sym(s), freq(f), left(nullptr), right(nullptr) {}

	Node(int f, Node* l, Node* r) : sym(0), freq(f), left(l), right(r) {}

	~Node() {
		delete left;
		delete right;
	}
};

string unpackBits(const vector<unsigned char>& packed, int totalBits) {
	string result;
	for (int i = 0; i < packed.size(); i++) {
		unsigned char byte = packed[i];
		for (int j = 0; j < 8; j++) {
			if (result.size() >= totalBits) break;
			if (byte & (1 << (7 - j))) result += '1';
			else result += '0';
		}
	}
	return result;
}

vector<unsigned char> packBits(const string& bits) {

	vector<unsigned char> result;

	for (int i = 0; i < bits.size(); i += 8) {

		unsigned char byte = 0;

		for (int j = 0; j < 8 && i + j < bits.size(); j++) {
			if (bits[i + j] == '1') {
				byte = byte | 1 << (7 - j);
			}
		}
		result.push_back(byte);
	}
	return result;
}

vector<unsigned char> packModel(const map<unsigned char, int>& freqs) {

	vector<unsigned char> result;

	for (auto& pair : freqs) {
		result.push_back(pair.first);
		result.push_back(pair.second);
	}
	return result;
}

map<unsigned char, int> unpackModel(const vector<unsigned char>& packedModel) {

	map<unsigned char, int> frequencies;

	for (int i = 0; i < packedModel.size(); i += 2) {

		unsigned char symbol = packedModel[i];
		int freq = packedModel[i + 1];
		frequencies[symbol] = freq;
	}
	return frequencies;
}

Node* buildTree(const map<unsigned char, int>& freqs) {

	// Вектор указателей на узлы дерева (символ + частота)
	vector<Node*> nodes;

	// Заполняем дерево, создавая новые узлы, на основе данных из мапы
	for (auto& pair : freqs) {
		nodes.push_back(new Node(pair.first, pair.second));
	}

	// Преобразуем дерево объединяя минимумы, пока не останется один элемент, он будет корнем потом
	while (nodes.size() > 1) {

		int min1 = 0;
		int min2 = 1;

		if (nodes[min2]->freq > nodes[min1]->freq) {
			swap(min1, min2);
		}

		// Ищем два минимума
		for (int i = 2; i < nodes.size(); i++) {
			if (nodes[i]->freq < nodes[min1]->freq) {
				min2 = min1;
				min1 = i;
			}
			else if (nodes[i]->freq < nodes[min2]->freq) {
				min2 = i;
			}
		}

		//Создаем раодительский элемент двух минимумов
		Node* left = nodes[min1];
		Node* right = nodes[min2];
		Node* parent = new Node(left->freq + right->freq, left, right);

		// Надо аккуртно теперь удалить минимумы, по порядку чтобы ниче не съехало
		if (min1 > min2) {
			nodes.erase(nodes.begin() + min1);
			nodes.erase(nodes.begin() + min2);
		}
		else {
			nodes.erase(nodes.begin() + min2);
			nodes.erase(nodes.begin() + min1);
		}

		// Вставляем новый (объежиненный) элемент в наш вектор указателей на элементы дерева
		nodes.push_back(parent);
	}
	// Корень, последний неудаленный элемент
	Node* root = nodes[0];

	return root;
}

vector <unsigned char> encodeHuffman(const vector<unsigned char>& data) {

	// Считаем частоты
	map<unsigned char, int> freqs;
	for (auto c : data) freqs[c]++;

	// Упаковка 2 байта на символ, первый - символ, 2 - частота вхождений
	vector<unsigned char> packedModel;
	for (auto& p : freqs) {
		packedModel.push_back(p.first);
		packedModel.push_back(p.second);
	}

	// Строим дерево на основе частот из мапы, вернем корень
	Node* root = buildTree(freqs);

	// Пора присвоить коды, у корневого элемента кода нет...эх, он отправная точка
	map<unsigned char, string> codes;
	makeCodes(root, "", codes);

	// Кодирование информации в биты 
	string bits;

	for (unsigned char s : data) {
		bits += codes[s];
	}
	vector<unsigned char> packedBits = packBits(bits);

	delete root;

	vector<unsigned char> result;
	int modelSize = packedModel.size();
	result.push_back(modelSize & 0xFF);
	result.push_back((modelSize >> 8) & 0xFF);

	result.insert(result.end(), packedModel.begin(), packedModel.end());
	result.insert(result.end(), packedBits.begin(), packedBits.end());

	return result;
}

void makeCodes(Node* root, string code, map<unsigned char, string>& codes) {

	if (root->left == nullptr && root->right == nullptr) {
		codes[root->sym] = code;
		return;
	}

	if (root->left != nullptr)
		makeCodes(root->left, code + "0", codes);
	if (root->right != nullptr)
		makeCodes(root->right, code + "1", codes);
}

vector <unsigned char> decodeHuffman(const vector<unsigned char>& compressed) {

	vector <unsigned char> decodedResult;

	int modelSize = compressed[0] | (compressed[1] << 8);

	map<unsigned char, int> freqs;
	for (int i = 0; i < modelSize; i += 2) {
		unsigned char sym = compressed[2 + i];
		int freq = compressed[2 + i + 1];
		freqs[sym] = freq;
	}

	Node* root = buildTree(freqs);

	// Опять коды
	map<unsigned char, string> codes;
	makeCodes(root, "", codes);

	int totalBits = 0;
	for (auto& p : freqs) {
		totalBits += p.second * codes[p.first].length();
	}
	vector<unsigned char> packedBits(compressed.begin() + 2 + modelSize, compressed.end());

	string bits = unpackBits(packedBits, totalBits);

	vector<unsigned char> result;
	Node* current = root;

	for (unsigned char bit : bits) {
		if (bit == '0') {
			current = current->left;
		}
		else {
			current = current->right;
		}

		if (current->left == nullptr && current->right == nullptr) {
			result.push_back(current->sym);
			current = root;
		}
	}
	delete root;

	return result;
}