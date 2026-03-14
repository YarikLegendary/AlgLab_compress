#include "compress.h"
#include "funcs_text.h"

vector <unsigned char> encodeRLE(vector <unsigned char>& data) {

	vector <unsigned char> CompressedText;

	for (int i = 0; i < data.size();) {

		// Определим тип последовательности
		int length = 1;
		while (i + length < data.size() && data[i] == data[i + length]) {
			length++;
		}

		// Повторяющаяся последовательность
		if (length > 1) {
			CompressedText.push_back(static_cast<unsigned char>(length));
			CompressedText.push_back(data[i]);
			i += length;
		}
		else { // Неповторяющаяся последовательность
			int start = i;
			while (i + 1 < data.size() && data[i] != data[i + 1]) {
				if (i + 2 < data.size() && data[i + 1] == data[i + 2]) {
					break;
				}
				i++; 
			}
			length = i - start + 1;
			CompressedText.push_back(static_cast<unsigned char>(0x80 | length));

			for (int k = start; k <= i; k++) {
				CompressedText.push_back(data[k]);
			}
			i++;
		}
	}
	return CompressedText;
}

vector <unsigned char> decodeRLE(const vector<unsigned char>& data) {

	vector <unsigned char> newData;

	for (int i = 0; i < data.size(); ) {

		if (data[i] & 0x80) {

			int length = data[i] - 0x80;

			for (int k = 0; k < length; k++) {
				newData.push_back(data[i + 1 + k]);
			}
			i += 1 + length;
		}
		else {
			int quantity = data[i];
			char symbol = data[i + 1];

			for (int k = 0; k < quantity; k++) {
				newData.push_back(symbol);
			}
			i += 2;
		}
	}
	cout << "\nДанные: ";
	for (auto sym : newData) {
		cout << sym;
	}
	cout << "\n";

	return newData;
}

double entropy(const vector<unsigned char>& data, int qtBlocks) {

	// Пофильтруем от смайликов всяких
	vector<unsigned char> filteredData;

	for (unsigned char sym : data) {
		if (sym <= 127) {
			filteredData.push_back(sym);
		}
	}
	
	double Entropy = 0.0;

	map<vector<unsigned char>, int> frequency;
	int totalQt = filteredData.size() / qtBlocks;

	for (int i = 0; i < filteredData.size() - qtBlocks + 1; i += qtBlocks) {

		vector<unsigned char> block;

		for (int j = 0; j < qtBlocks;j++) {
			block.push_back(filteredData[i+j]);
		}
		frequency[block]++;
	}

	for (auto& pair : frequency) {
		double p = (double)pair.second / totalQt;
		Entropy += -p * log2(p);
	}

	cout << "Энтропия: " << fixed << setprecision(2) << Entropy << (qtBlocks == 1? " (бит на символ)" : " (бит на блок символов)") << endl;

	return Entropy;
}

vector<int> MTF(const vector<unsigned char>& data) {

	list<unsigned char> alphabet;

	for (int i = 0; i < 127; i++) {
		alphabet.push_back(static_cast<unsigned char>(i));
	}

	vector<int> encodeResult;

	encodeResult.reserve(data.size());

	for (int i = 0; i < data.size();i++) {

		auto ptr = find(alphabet.begin(), alphabet.end(), data[i]);
		int pos = distance(alphabet.begin(), ptr);

		encodeResult.push_back(pos);

		alphabet.erase(ptr);
		alphabet.push_front(data[i]);
	}

	return encodeResult;
}

vector<unsigned char> decodeMTF(const vector<int>& data) {

	list<unsigned char> alphabet;

	for (int i = 0; i < 127; i++) {
		alphabet.push_back(static_cast<unsigned char>(i));
	}
	vector<unsigned char> decodeResult;
	decodeResult.reserve(data.size());

	for (int i = 0; i < data.size();i++) {

		auto ptr = alphabet.begin();
		int pos = data[i];

		advance(ptr, pos);

		unsigned char sym = *ptr;
		decodeResult.push_back(sym);

		alphabet.erase(ptr);
		alphabet.push_front(sym);
	}

	return decodeResult;
}
