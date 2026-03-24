#pragma once
#include "funcs.h"

vector <unsigned char> encodeRLE(vector <unsigned char>& data);
vector <unsigned char> decodeRLE(const vector<unsigned char>& data);

vector<int> MTF(const vector<unsigned char>& data);
vector<unsigned char> decodeMTF(const vector<int>& data);

template<typename T>
double entropy(const vector<T>& data, int qtBlocks) {

	vector<unsigned char> encodedData;
	encodedData.reserve(data.size());

	for (const T& sym : data) {
		if (sym >= 0 && sym <= 255) {
			encodedData.push_back(static_cast<unsigned char>(sym));
		}
	}

	double Entropy = 0.0;

	map<vector<unsigned char>, int> frequency;
	int totalQt = encodedData.size() / qtBlocks;

	for (size_t i = 0; i < encodedData.size() - qtBlocks + 1; i += qtBlocks) {

		vector<unsigned char> block; block.reserve(qtBlocks);

		for (size_t j = 0; j < qtBlocks;j++) {
			block.push_back(encodedData[i + j]);
		}
		frequency[block]++;
	}

	for (auto& pair : frequency) {
		double p = (double)pair.second / totalQt;
		Entropy += -p * log2(p);
	}

	cout << "Entropy: " << fixed << setprecision(2) << Entropy << (qtBlocks == 1 ? " (bits per symbol)" : " (bits per symbol)") << endl;

	return Entropy;
}

vector <unsigned char> encodeRLERGB(vector <unsigned char>& data);
vector <unsigned char> decodeRLERGB(const vector<unsigned char>& data);
